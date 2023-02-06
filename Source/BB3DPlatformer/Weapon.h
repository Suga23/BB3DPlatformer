// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

class APlayerCharacter;

/**
 * 
 */

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EIT_Physical UMETA(DisplayName = "Physical"),
	EIT_Magic UMETA(DisplayName = "Magic"),

	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BB3DPLATFORMER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackCooldownTimer;

	bool bComboSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* AttackImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundBase* AttackImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bHoldingButton;
	//VisibleAnywhere, BlueprintReadWrite

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	//TArray<AWeaponSkill*> Skills;

	//int32 Skills{ 5 };

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileShootPoint;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;

	APlayerCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FirstSkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SecondSkillMontage;

private:


public:

	FORCEINLINE USoundBase* GetAttackSound() const { return AttackSound; }
	FORCEINLINE USoundBase* GetAttackImpactSound() const { return AttackImpactSound; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE UParticleSystem* GetAttackImpactParticles() const { return AttackImpactParticles; }
	
	FORCEINLINE float GetAttackDamage() const { return AttackDamage; }
	FORCEINLINE void SetAttackDamage(float AtcDamage) { AttackDamage = AtcDamage; }
	
	FORCEINLINE float GetCooldownTimer() const { return AttackCooldownTimer; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE bool GetComboSkill() const { return bComboSkill; }

	FORCEINLINE UAnimMontage* GetFirstSkillMontage() const { return FirstSkillMontage; }
	FORCEINLINE UAnimMontage* GetSecondSkillMontage() const { return SecondSkillMontage; }

	//void SetWeaponTypeProperties(EWeaponType WeaponT, AWeapon* EquippedWeapon, APlayerCharacter* Char);

	//void PlayAttackSound();
};
