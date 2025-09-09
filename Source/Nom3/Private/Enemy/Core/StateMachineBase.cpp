// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/StateMachineBase.h"

UStateMachineBase::UStateMachineBase() :
	bReadyToExecute(false),
	ElapsedTimeInState(0),
	LimitTimeInState(-1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineBase::EnterState()
{
	//열림
	bReadyToExecute = true;

	//지나간 시간
	ElapsedTimeInState = 0;
}

void UStateMachineBase::ExecuteState()
{
	//진입 이벤트 발생
	if (bReadyToExecute == false)
	{
		EnterState();
	}
	
	//지나간 시간 누적
	ElapsedTimeInState += GetWorld()->GetDeltaSeconds();
}

void UStateMachineBase::ExitState()
{
	//초기화
	bReadyToExecute = false;
}