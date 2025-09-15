// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShankFollowPathStateMachine.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ShankBase.h"

UScoutShankFollowPathStateMachine::UScoutShankFollowPathStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UScoutShankFollowPathStateMachine::EnterState()
{
	Super::EnterState();
	
	if (OwnerShank == false)
	{
		return;
	}

	//목표 위치
	if (FVector Value; OwnerShank->PathQueue.Dequeue(Value))
	{
		OwnerShank->TargetLocation = Value;	
	}

	//초반 가속
	OwnerShank->DroneMoveComp->ThrottleThrustByLevel(20);
}

void UScoutShankFollowPathStateMachine::ExecuteState()
{
	Super::ExecuteState();
	
	if (OwnerShank == false)
	{
		return;
	}
	
	//목표까지의 차이 벡터
	const FVector TargetDiff = OwnerShank->TargetLocation - OwnerShank->GetActorLocation();

	//목표로 향하는 방향
	const FVector TargetDir = TargetDiff.GetSafeNormal();
	
	//순항 속도로 추력 조절
	OwnerShank->DroneMoveComp->ThrottleToCruise();
	
	//추력 편향
	OwnerShank->DroneMoveComp->VectorThrust(TargetDir);

	//목표까지의 거리가 1.5m 미만이라면
	if (TargetDiff.Length() < 200)
	{
		//역추진
		OwnerShank->SHANK_STATE = EShankState::ReverseThrust;
	}
}

void UScoutShankFollowPathStateMachine::ExitState()
{
	Super::ExitState();
	
	if (OwnerShank == false)
	{
		return;
	}
}