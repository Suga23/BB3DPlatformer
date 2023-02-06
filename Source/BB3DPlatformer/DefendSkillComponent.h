// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DefendSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BB3DPLATFORMER_API UDefendSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDefendSkillComponent();

	class APlayerCharacter* PlayerCharacter;

	APlayerController* PlayerCharController;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffParameters, meta = (AllowPrivateAccess = "true"))
	float IncreasedAttackDamage = 30.f;

	float InitialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffParameters, meta = (AllowPrivateAccess = "true"))
	float BuffTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffParameters, meta = (AllowPrivateAccess = "true"))
	float MagicShieldTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuffParameters, meta = (AllowPrivateAccess = "true"))
	float MagicShieldAmount = 50.f;

	FTimerHandle AttackBuffTimer;
	FTimerHandle MagicShieldTimer;

	void IncreaseAttackDamage();

	void ResetAttack();

	void ResetMagicShield();

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetInitialDamage(float BaseWeaponDamage);

	UFUNCTION(BlueprintCallable)
	void BuffAttackDamage();

	UFUNCTION(BlueprintCallable)
	void MagicShield();
};
