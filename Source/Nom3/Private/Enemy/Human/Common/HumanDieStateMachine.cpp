// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanDieStateMachine.h"

UHumanDieStateMachine::UHumanDieStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHumanDieStateMachine::EnterState()
{
	Super::EnterState();
}

void UHumanDieStateMachine::ExecuteState()
{
	Super::ExecuteState();
}

void UHumanDieStateMachine::ExitState()
{
	Super::ExitState();
}