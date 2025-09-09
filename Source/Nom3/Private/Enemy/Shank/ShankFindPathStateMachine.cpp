// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankFindPathStateMachine.h"

#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ShankBase.h"

UShankFindPathStateMachine::UShankFindPathStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UShankFindPathStateMachine::BeginPlay()
{
	Super::BeginPlay();
}

void UShankFindPathStateMachine::EnterState()
{
	Super::EnterState();
	
	if (OwnerShank == false)
	{
		return;
	}
	
	//목표 위치 결정
	DecideTargetLocation();
	
	//제한 시간 초기화
	LimitTimeInState = 5;
}

void UShankFindPathStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (OwnerShank == false)
	{
		return;
	}

	//제한 시간 초과
	if (ElapsedTimeInState > LimitTimeInState)
	{
		//상태 머신 전환
		OwnerShank->SHANK_STATE = EShankState::FollowPath;
	}
}

void UShankFindPathStateMachine::ExitState()
{
	Super::ExitState();

	if (OwnerShank == false)
	{
		return;
	}
}

void UShankFindPathStateMachine::DecideTargetLocation() const
{
	//목표의 방향 요소 벡터
	const FVector UpDir = OwnerShank->TargetPawn->GetActorUpVector();
	const FVector RightDir = OwnerShank->TargetPawn->GetActorRightVector();
	
	//북극에서 적도를 향해 랜덤 범위만큼
	FVector RandDir = UpDir.RotateAngleAxis(FMath::RandRange(60, 85), RightDir);
	
	//북극축을 기준으로 랜덤 범위만큼 회전
	RandDir = RandDir.RotateAngleAxis(FMath::RandRange(-180, 180), UpDir);
	
	//플레이어와 유지하길 원하는 거리
	const float Distance = OwnerShank->KeepDistance;

	//적당한 거리에 적당한 눈높이에 있는 360도 범위 내 좌표를 가지게 된다
	OwnerShank->TargetLocation = OwnerShank->TargetPawn->GetActorLocation() + RandDir * FMath::FRandRange(Distance - 50, Distance + 50);
}