// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

ARangeWeapon::ARangeWeapon()
{

}

void ARangeWeapon::WeaponAttack_Implementation(APlayerCharacter* Char, AWeapon* EquippedWeapon, AActor* Shooter, AController* ShooterController)
{
	FVector Location = Char->GetProjectileShootPoint()->GetComponentLocation();
	FRotator Rotation = Char->GetProjectileShootPoint()->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(Char);

	if (EquippedWeapon->GetMuzzleFlash())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			EquippedWeapon,
			EquippedWeapon->GetMuzzleFlash(),
			Location,
			Rotation);
	}

	if (EquippedWeapon->GetAttackSound())
	{
		UGameplayStatics::PlaySoundAtLocation(
			Char,
			EquippedWeapon->GetAttackSound(),
			GetActorLocation());
	}
}
