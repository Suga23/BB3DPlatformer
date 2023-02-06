// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTest.h"

// Sets default values
AEnemyTest::AEnemyTest() :
	Health(100.f),
	MagicShieldPoints(50.f),
	bUnderMagicShield(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyTest::BeginPlay()
{
	Super::BeginPlay();
	
}

float AEnemyTest::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (/*bUnderMagicShield &&*/ MagicShieldPoints > 0)
	{
		MagicShieldPoints -= DamageAmount;
		//Health = MagicShieldPoints + Health - DamageAmount;

		if (MagicShieldPoints <= 0)
		{

		}

		UE_LOG(LogTemp, Warning, TEXT("Enemy Shield %f"), MagicShieldPoints);
		UE_LOG(LogTemp, Warning, TEXT("Enemy Health %f"), Health);
	}
	else 
	{
		Health -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Enemy Health %f"), Health);
	}

	
	if (Health <= 0.f)
	{
		//Health = 0.f;
		UE_LOG(LogTemp, Warning, TEXT("Enemy died!"));
	}
	return DamageAmount;
}

void AEnemyTest::UpdateHealth(float HpChange)
{
	if (Health <= 0.f)
	{
		Health = 0.f;
		UE_LOG(LogTemp, Warning, TEXT("Enemy died!"));
	}
	else
	{
		//Health -= HpChange;
		UE_LOG(LogTemp, Warning, TEXT("Enemy Health %f"), Health);
	}
}

void AEnemyTest::TemporaryHp()
{
	Health += 50.f;
}

// Called every frame
void AEnemyTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

