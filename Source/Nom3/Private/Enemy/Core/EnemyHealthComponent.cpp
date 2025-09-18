// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyHealthComponent.h"

UEnemyHealthComponent::UEnemyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyHealthComponent::Init(const int32 HP)
{
	MaxHP = HP;
	CurrentHP = HP;
}
