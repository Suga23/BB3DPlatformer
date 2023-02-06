// Fill out your copyright notice in the Description page of Project Settings.


#include "DefendSkillComponent.h"
#include "PlayerCharacter.h"
#include "Weapon.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UDefendSkillComponent::UDefendSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDefendSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UDefendSkillComponent::IncreaseAttackDamage()
{
	if (PlayerCharacter->GetEquippedWeapon())
	{
		PlayerCharacter->GetEquippedWeapon()->SetAttackDamage(IncreasedAttackDamage);
	}
}

void UDefendSkillComponent::ResetAttack()
{
	if (PlayerCharacter == nullptr) return;

	PlayerCharacter->GetEquippedWeapon()->SetAttackDamage(InitialDamage);
}

void UDefendSkillComponent::ResetMagicShield()
{
	PlayerCharacter->SetMagicShield(PlayerCharacter->GetMagicShield() - 50);

	UE_LOG(LogTemp, Warning, TEXT("Magic Shield = %f"), PlayerCharacter->GetMagicShield());
}

// Called every frame
void UDefendSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDefendSkillComponent::SetInitialDamage(float BaseWeaponDamage)
{
	InitialDamage = BaseWeaponDamage;
}

void UDefendSkillComponent::BuffAttackDamage()
{
	if (PlayerCharacter == nullptr) return;

	PlayerCharacter->GetWorldTimerManager().SetTimer(
		AttackBuffTimer,
		this,
		&UDefendSkillComponent::ResetAttack,
		BuffTime);

	IncreaseAttackDamage();
}

void UDefendSkillComponent::MagicShield()
{
	if (PlayerCharacter == nullptr) return;

	PlayerCharacter->SetUnderShieldBarier(true);

	PlayerCharacter->GetWorldTimerManager().SetTimer(
		MagicShieldTimer,
		this,
		&UDefendSkillComponent::ResetMagicShield,
		MagicShieldTime);

	PlayerCharacter->SetMagicShield(MagicShieldAmount);
	UE_LOG(LogTemp, Warning, TEXT("Magic Shield = %f"), PlayerCharacter->GetMagicShield());
}

