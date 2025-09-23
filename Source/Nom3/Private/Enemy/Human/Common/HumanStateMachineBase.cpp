// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanStateMachineBase.h"
#include "Enemy/Human/Common/HumanBase.h"

UHumanStateMachineBase::UHumanStateMachineBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHumanStateMachineBase::BeginPlay()
{
	Super::BeginPlay();

	//인간형 에너미 획득
	OwnerHuman = Cast<AHumanBase>(GetOwner());
}