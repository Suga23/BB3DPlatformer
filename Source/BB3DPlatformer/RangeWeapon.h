// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponAttackInterface.h"
#include "WeaponType.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BB3DPLATFORMER_API ARangeWeapon : public AWeapon, public IWeaponAttackInterface
{
	GENERATED_BODY()
	
public:
	ARangeWeapon();

	virtual void WeaponAttack_Implementation(class APlayerCharacter* Char, class AWeapon* EquippedWeapon, AActor* Shooter, AController* ShooterController) override;

private:

	AWeapon* WeaponTest;
};
