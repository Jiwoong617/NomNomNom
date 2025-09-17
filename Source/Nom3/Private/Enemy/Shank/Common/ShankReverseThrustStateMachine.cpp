// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/ShankReverseThrustStateMachine.h"

#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/Common/ShankBase.h"

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
	LimitTimeInState = 0.5;
}

void UShankReverseThrustStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (OwnerShank == false)
	{
		return;
	}

	//역추진 가속도 연산 - LimitTimeInState가 지나면 속도가 0이 되는 2가지 스타일의 가속도 공식
	//0에서 LimitTimeInState까지의 적분 결과는 항상 -InitVelocity로 결국 속도는 0이 된다

	//초반에 강한 역추진 - 최후에는 가속도 최소, 별도의 처리는 필요없음
	const float OutAccel = -2 * InitVelocity / LimitTimeInState * (1 - ElapsedTimeInState / LimitTimeInState);

	//후반에 강한 역추진 - 최후에는 가속도 최대, 별도의 처리가 필요함
	const float InAccel = -2 * InitVelocity * ElapsedTimeInState / (LimitTimeInState * LimitTimeInState);

	//결과로 얻은 가속도를 기반으로 역추진 적용
	OwnerShank->DroneMoveComp->ReverseVectorThrust(FMath::Abs(InAccel));

	//역추진 제한시간 초과
	if (ElapsedTimeInState > LimitTimeInState)
	{
		if (OwnerShank->PathQueue.IsEmpty())
		{
			//추진력를 0으로 초기화해줘야 오류가 발생하지 않는다
			OwnerShank->DroneMoveComp->ThrottleOff();

			//상태 머신 전환
			OwnerShank->ChangeCurrentStateMachine(OwnerShank->FindPathStateMachine);
		}
		else
		{
			//상태 머신 전환
			OwnerShank->ChangeCurrentStateMachine(OwnerShank->FollowPathStateMachine);
		}
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