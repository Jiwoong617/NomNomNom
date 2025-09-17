// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerFpsAnimation.h"
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
		CameraRotX = Owner->GetControlRotation().Pitch;

		MovingState = Owner->GetMovingState();
		ActionState = Owner->GetActionState();
	}
}

void UPlayerFpsAnimation::PlayGunshotAnim(UAnimMontage* Montage)
{
	Montage_Play(Montage);
}
