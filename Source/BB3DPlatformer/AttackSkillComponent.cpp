// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSkillComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UAttackSkillComponent::UAttackSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (PlayerCharacter->GetEquippedWeapon() != nullptr)
	{
		auto EquippedWea = Cast<AMeleeWeapon>(PlayerCharacter->GetEquippedWeapon());
		EquippedWea->GetMeleeWeaponCollision()->OnComponentBeginOverlap.AddDynamic(
			this,
			&UAttackSkillComponent::OnHeavyStrikeOverlap);
	}*/
}

void UAttackSkillComponent::BackstabCharacter(AActor* OtherActor)
{
	ACharacter* PushChar = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter->GetActorLocation().X > OtherActor->GetActorLocation().X)
	{

		

		FVector Trajectory = FVector(OtherActor->GetActorLocation().X * -0.5, 0, OtherActor->GetActorLocation().X * 0.5);

		PushChar->LaunchCharacter(Trajectory, true, true);
	}
	else
	{
		//FVector Trajectory = FVector(1000, 0, -750);

		FVector Trajectory = FVector(OtherActor->GetActorLocation().X * 0.5, 0, OtherActor->GetActorLocation().X * 0.5);

		PushChar->LaunchCharacter(Trajectory, true, true);
	}
	
}

void UAttackSkillComponent::OnHeavyStrikeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HeavyStrike(OtherActor);
}

void UAttackSkillComponent::HeavyStrike(AActor* Target)
{
	if (Target == nullptr) return;

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) return; 
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	
	UGameplayStatics::ApplyDamage(
		Target,
		HeavyStrikeDamage,
		MyOwnerInstigator,
		MyOwnerInstigator,
		UDamageType::StaticClass());
	
	if (Target)
	{
		BackstabCharacter(Target);
	}
}

// Called every frame
void UAttackSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UAttackSkillComponent::ActivateHeavyStrike()
{
	auto EquippedW = Cast<AMeleeWeapon>(PlayerCharacter->GetEquippedWeapon());
	EquippedW->GetMeleeWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	auto EquippedWea = Cast<AMeleeWeapon>(PlayerCharacter->GetEquippedWeapon());
	EquippedWea->GetMeleeWeaponCollision()->OnComponentBeginOverlap.AddDynamic(
		this,
		&UAttackSkillComponent::OnHeavyStrikeOverlap);
}

void UAttackSkillComponent::DeactivateHeavyStrike()
{
	auto EquippedW = Cast<AMeleeWeapon>(PlayerCharacter->GetEquippedWeapon());
	EquippedW->GetMeleeWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}