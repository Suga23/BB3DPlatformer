// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.h"

AWeapon::AWeapon() :
	AttackDamage(10),
	AttackRange(1'000.f),
	AttackCooldownTimer(10.f)
	//WeaponType(EWeaponType::EIS_MAX)
{
	PrimaryActorTick.bCanEverTick = true;

	/*ProjectileShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Shoot Point"));
	ProjectileShootPoint->SetupAttachment(GetRootComponent());*/
}

