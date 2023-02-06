// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponAttackInterface.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BB3DPLATFORMER_API AMeleeWeapon : public AWeapon, public IWeaponAttackInterface
{
	GENERATED_BODY()
	
public:

	AMeleeWeapon();

protected:
	virtual void WeaponAttack_Implementation(class APlayerCharacter* Char, class AWeapon* EquippedWeapon, AActor* Shooter, AController* ShooterController) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WeaponCollision;

public:
	FORCEINLINE UBoxComponent* GetMeleeWeaponCollision() const { return WeaponCollision; }
};
