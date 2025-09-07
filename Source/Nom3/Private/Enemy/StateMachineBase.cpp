// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/StateMachineBase.h"

UStateMachineBase::UStateMachineBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStateMachineBase::EnterState()
{
	bReadyToExecute = true;
}

void UStateMachineBase::ExecuteState()
{
	
}

void UStateMachineBase::ExitState()
{
	bReadyToExecute = false;
}