// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankReverseThrustStateMachine.h"

#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ShankBase.h"

UShankReverseThrustStateMachine::UShankReverseThrustStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UShankReverseThrustStateMachine::EnterState()
{
	Super::EnterState();

	if (OwnerShank == false)
	{
		return;
	}

	//초기 속도 저장
	InitVelocity = OwnerShank->DroneMoveComp->Velocity.Length();

	//역추진 제한 시간 초기화
	LimitTimeInState = 2;
}

void UShankReverseThrustStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (OwnerShank == false)
	{
		return;
	}

	//역추진 가속도 연산 - LimitTimeInState가 지나면 속도가 0이 되는 가속도 공식
	const float Accel = -2 * InitVelocity / LimitTimeInState + (1 - ElapsedTimeInState / LimitTimeInState);

	//결과로 얻은 가속도를 기반으로 역추진 적용
	OwnerShank->DroneMoveComp->ReverseVectorThrust(FMath::Abs(Accel));

	//역추진 제한시간 초과
	if (ElapsedTimeInState > LimitTimeInState)
	{
		//상태 머신 전환
		OwnerShank->SHANK_STATE = EShankState::FindPath;
	}
}

void UShankReverseThrustStateMachine::ExitState()
{
	Super::ExitState();

	if (OwnerShank == false)
	{
		return;
	}
}