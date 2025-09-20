// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanIdleStateMachine.h"
#include "Enemy/Human/Common/HumanBase.h"

UHumanIdleStateMachine::UHumanIdleStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHumanIdleStateMachine::EnterState()
{
	Super::EnterState();

	//2초 후에 Move 상태로 전환할 예정
	LimitTimeInState = 2;
}

void UHumanIdleStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (ElapsedTimeInState > LimitTimeInState)
	{
		//이동 상태 머신으로 전환
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->MoveStateMachine);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, TEXT("UHumanIdleStateMachine::ExecuteState"));
}

void UHumanIdleStateMachine::ExitState()
{
	Super::ExitState();
}