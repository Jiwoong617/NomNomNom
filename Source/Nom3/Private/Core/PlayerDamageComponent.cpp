// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/PlayerDamageComponent.h"

#include "Core/NomPlayer.h"

UPlayerDamageComponent::UPlayerDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerDamageComponent::OnHitBody(FFireInfo& Info)
{
	if (const auto OwnerPlayer = Cast<ANomPlayer>(GetOwner()))
	{
		OwnerPlayer->OnDamaged(Info);
	}
}

void UPlayerDamageComponent::OnHitHead(FFireInfo& Info)
{
	if (const auto OwnerPlayer = Cast<ANomPlayer>(GetOwner()))
	{
		OwnerPlayer->OnCriticalDamaged(Info);
	}
}