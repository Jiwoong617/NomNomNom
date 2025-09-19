// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerTpsAnimation.h"
#include "Core/NomPlayer.h"

void UPlayerTpsAnimation::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerTpsAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ANomPlayer>(TryGetPawnOwner());
}

void UPlayerTpsAnimation::PlaySkillAnim(UAnimMontage* montage)
{
	Montage_Play(montage);
}
