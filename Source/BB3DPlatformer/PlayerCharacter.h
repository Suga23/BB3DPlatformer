// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "Weapon.h"
#include "MeleeWeapon.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	//ECS_Equipping UMETA(DisplayName = "Equipping"),
	ECS_Stunned UMETA(DisplayName = "Stunned"),

	ECS_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EPlayerPosition : uint8
{
	EPP_Left UMETA(DisplayName = "Left"),
	EPP_Right UMETA(DisplayName = "Right"),

	EPP_MAX UMETA("DefaultMax")
};

UCLASS()
class BB3DPLATFORMER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CharacterMove(float Value);

	void StartJumping();

	void AttackButtonPressed();
	void AttackButtonReleased();
	void FirstSkillButtonPressed();
	void SecondSkillButtonPressed();

	AWeapon* EquipDefaultWeapon();

	void EquipWeapon(AWeapon* WeaponToEquip);

	void GetWeaponTypeProperties(EWeaponType WeaponT, AWeapon* EquippedWeapon);

	void StartAttack();

	UFUNCTION()
	void ResetAttack();

	void StartCooldownTimer();

	UFUNCTION(BlueprintCallable)
	void ActivateMeleeWeapon();

	UFUNCTION(BlueprintCallable)
	void DeactivateMeleeWeapon();

	UFUNCTION()
	void OnMeleeWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void RotateCharacter();

	void RotateShootPoint();

	void CrouchButtonPressed();

	void CapsuleHalfHeight(float DeltaTime);

	virtual float TakeDamage(
		float DamageAmount, 
		FDamageEvent const& DamageEvent, 
		AController* EventInstigator, 
		AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	void Stun();

	void EndStun();

	UFUNCTION(BlueprintCallable)
	void UseFirstSkillMontage();

	UFUNCTION(BlueprintCallable)
	void UseSecondSkillMontage();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	bool bTrackItems;

	APlayerController* PlayerCharController;

	int8 OverlappedItemCount;

	bool bPlayPickupSound;
	bool bPlayUsingSound;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	void ResetPickupSoundTimer();
	void ResetUsingSoundTimer();

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float ResetPickupSoundTime; // Время ожидания, до того как может сыграть ещё один звук поднятия предмета.

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float ResetUsingSoundTime; // Время ожидания, до того как может сыграть ещё один звук экипирования предмета.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> EquippedWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileShootPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundBase* MeleeImpactSound;

	bool bCanAttack;
	bool bAttackButtonPressed;
	FTimerHandle AttackTimer;

	class AMeleeWeapon* MeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	float CurrentCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	
	float MagicShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bDead;

	ECombatState CombatState;

	EPlayerPosition PlayerPosition;

	// bool bFirstSkillButtonPressed;

	bool bUnderShield;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void IncrementOverlappedItemCount(int8 Amount);

	void TakeItem(AItem* Item);

	FORCEINLINE bool ShouldPlayPickupSound() const { return bPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bPlayUsingSound; }

	void StartPickupSoundTimer();
	void StartUsingSoundTimer();

	// FORCEINLINE AWeapon* GetCharacterEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE USceneComponent* GetProjectileShootPoint() const { return ProjectileShootPoint; }

	APlayerController* GetPlayerCharacterController() const { return PlayerCharController; }
	AController* GetController();

	UFUNCTION(BlueprintCallable)
	void PlayMeleeAttackMontage();

	//APlayerController* GetPlayerCharacterController();

	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }

	FORCEINLINE EPlayerPosition GetPlayerPosition() const { return PlayerPosition; }

	void DoMeleeDamage(AActor* Target);

	FORCEINLINE bool GetCrouching() const { return bCrouching; }

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE float GetMagicShield() const { return MagicShield; }
	FORCEINLINE void SetMagicShield(float MagicShieldAmount) { MagicShield = MagicShieldAmount; }

	FORCEINLINE void SetUnderShieldBarier(bool bShield) { bUnderShield = bShield; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UDefendSkillComponent* DefendSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAttackSkillComponent* AttackSkill;
};
