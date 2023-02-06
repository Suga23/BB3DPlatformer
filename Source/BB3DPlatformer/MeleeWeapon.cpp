// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"

AMeleeWeapon::AMeleeWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponCollision->SetupAttachment(RootComponent, FName("WeaponCollisionSocket"));

	/*WeaponCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMeleeWeapon::OnMeleeWeaponOverlap);*/

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Поскольку нужно только на события соприкосновения, когда box(оружие) соприкасается с pawn.
	WeaponCollision->SetCollisionResponseToChannel( // На что отвечает коллизия.
		ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap);
}

void AMeleeWeapon::WeaponAttack_Implementation(APlayerCharacter* Char, AWeapon* EquippedWeapon, AActor* Shooter, AController* ShooterController)
{
	if (EquippedWeapon == nullptr) return;
	if (EquippedWeapon->GetWeaponType() != EWeaponType::EIS_Melee) return;
	if (Char->GetCombatState() != ECombatState::ECS_Unoccupied) return;

	Char->PlayMeleeAttackMontage();
}