// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackSkillComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BB3DPLATFORMER_API UAttackSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackSkillComponent();

	class APlayerCharacter* PlayerCharacter;

	APlayerController* PlayerCharController;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BackstabCharacter(AActor* OtherActor);

private:

	float HeavyStrikeDamage = 15.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION(BlueprintCallable)
	void HeavyStrike(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void ActivateHeavyStrike();

	UFUNCTION(BlueprintCallable)
	void DeactivateHeavyStrike();

	UFUNCTION()
	void OnHeavyStrikeOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
