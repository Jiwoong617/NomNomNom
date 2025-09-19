// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/DroneFollowPathStateMachine.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/Common/DroneBase.h"

UDroneFollowPathStateMachine::UDroneFollowPathStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDroneFollowPathStateMachine::EnterState()
{
	Super::EnterState();
	
	if (OwnerDrone == false)
	{
		return;
	}

	//목표 위치
	if (FVector Value; OwnerDrone->PathQueue.Dequeue(Value))
	{
		OwnerDrone->Destination = Value;	
	}

	//초반 가속
	OwnerDrone->DroneMoveComp->ThrottleThrustByLevel(20);
}

void UDroneFollowPathStateMachine::ExecuteState()
{
	Super::ExecuteState();
	
	if (OwnerDrone == false)
	{
		return;
	}
	
	//목표까지의 차이 벡터
	const FVector TargetDiff = OwnerDrone->Destination - OwnerDrone->GetActorLocation();

	//목표로 향하는 방향
	const FVector TargetDir = TargetDiff.GetSafeNormal();
	
	//순항 속도로 추력 조절
	OwnerDrone->DroneMoveComp->ThrottleToCruise();
	
	//추력 편향
	OwnerDrone->DroneMoveComp->VectorThrust(TargetDir);

	//목표까지의 거리가 2m 미만이라면
	if (TargetDiff.Length() < 200)
	{
		//역추진 상태로 전환
		OwnerDrone->ChangeCurrentStateMachine(OwnerDrone->ReverseThrustStateMachine);
	}
}

void UDroneFollowPathStateMachine::ExitState()
{
	Super::ExitState();
	
	if (OwnerDrone == false)
	{
		return;
	}
}