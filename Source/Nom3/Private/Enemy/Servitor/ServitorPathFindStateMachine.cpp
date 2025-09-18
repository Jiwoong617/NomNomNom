// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Servitor/ServitorPathFindStateMachine.h"


// Sets default values for this component's properties
UServitorPathFindStateMachine::UServitorPathFindStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UServitorPathFindStateMachine::EnterState()
{
	Super::EnterState();
}

void UServitorPathFindStateMachine::ExecuteState()
{
	Super::ExecuteState();
}

void UServitorPathFindStateMachine::ExitState()
{
	Super::ExitState();
}

void UServitorPathFindStateMachine::DecideTargetLocation() const
{
}

void UServitorPathFindStateMachine::CalculatePaths() const
{
}
