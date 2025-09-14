// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerFpsAnimation.h"

#include <gsl/pointers>

#include "Core/NomPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerFpsAnimation::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerFpsAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ANomPlayer>(TryGetPawnOwner());
}

void UPlayerFpsAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner)
	{
		Speed = FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorForwardVector())/Owner->GetCharacterMovement()->GetMaxSpeed();
		Dir = FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorRightVector())/Owner->GetCharacterMovement()->GetMaxSpeed();
		bIsAir = Owner->GetCharacterMovement()->IsFalling();
		bIsAiming = Owner->GetIsAiming();

		MovingState = Owner->GetMovingState();
		ActionState = Owner->GetActionState();
	}
}
