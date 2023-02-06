// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PlayerChar == nullptr)
	{
		PlayerChar = Cast<APlayerCharacter>(TryGetPawnOwner());
	}

	if (PlayerChar)
	{
		bCrouching = PlayerChar->GetCrouching();

		FVector Velocity{ PlayerChar->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = PlayerChar->GetCharacterMovement()->IsFalling();

		if (PlayerChar->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerChar = Cast<APlayerCharacter>(TryGetPawnOwner());
}
