// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AProjectile::AProjectile() :
	ProjectileSpeed(1000.f),
	ProjectileInitialSpeed(1000.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;
	

	ProjectileSpeedComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Speed Component"));
	ProjectileSpeedComponent->MaxSpeed = ProjectileSpeed;
	ProjectileSpeedComponent->InitialSpeed = ProjectileInitialSpeed;
	//ProjectileSpeedComponent->bRotationFollowsVelocity = true;
	ProjectileSpeedComponent->bShouldBounce = true;
	ProjectileSpeedComponent->ProjectileGravityScale = 0;


	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	DamageSphere->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if (ExposionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this, 
				ExposionSound, 
				GetActorLocation());
		}
		if (ExplosionAnimation)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), 
				ExplosionAnimation, 
				HitResult.Location);
		}

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(
			OverlappingActors,
			ACharacter::StaticClass()); // Кому применяется урон.

		for (auto Actor : OverlappingActors)
		{
			UGameplayStatics::ApplyDamage(
				OtherActor, 
				AOEDamage,
				MyOwnerInstigator, 
				this, 
				DamageTypeClass);
		}
	}
	Destroy();
}
