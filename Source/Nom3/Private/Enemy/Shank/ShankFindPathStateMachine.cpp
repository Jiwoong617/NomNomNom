// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankFindPathStateMachine.h"

#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ShankBase.h"
#include "Kismet/KismetMathLibrary.h"

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

	//경로 계산
	CalculatePaths();
	
	//제한 시간 초기화
	LimitTimeInState = 1;
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
	constexpr float Distance = 1500;

	//적당한 거리에 적당한 눈높이에 있는 360도 범위 내 좌표를 가지게 된다
	OwnerShank->TargetLocation = OwnerShank->TargetPawn->GetActorLocation() + RandDir * FMath::FRandRange(Distance - 50, Distance + 50);
}

void UShankFindPathStateMachine::CalculatePaths() const
{
	if (OwnerShank == false)
	{
		return;
	}

	//경로 배열을 비운다
	OwnerShank->PathQueue.Empty();

	//시작 위치와 목표 위치
	const FVector StartLocation = OwnerShank->GetActorLocation();
	const FVector TargetLocation = OwnerShank->TargetLocation;

	//세그먼트 개수
	constexpr int32 NumSegments = 3;

	//디버그 드로우를 위한 배열
	TArray<FVector> DebugDrawPointArray;

	//경로 큐에 입력
	for (int32 i = 1; i < NumSegments; ++i)
	{
		//지그재그 규모
		const float RandomOffset = FMath::RandRange(200, 400);

		//세그먼트 중간점
		const float Alpha = static_cast<float>(i) / NumSegments;
		FVector IntermediatePoint = FMath::Lerp(StartLocation, TargetLocation, Alpha);

		//랜덤 포인트 획득
		IntermediatePoint += FVector::UpVector * RandomOffset;

		//중간 경로 삽입
		OwnerShank->PathQueue.Enqueue(IntermediatePoint);
		DebugDrawPointArray.Add(IntermediatePoint);
	}

	//최종 목표 삽입
	OwnerShank->PathQueue.Enqueue(TargetLocation);
	DebugDrawPointArray.Add(TargetLocation);

	//목표까지 가는 경로 디버그 드로우
	DrawDebugLine(GetWorld(), StartLocation, DebugDrawPointArray[0], FColor::Blue, false, 10, 0, 0);
	for (int32 i = 0; i < DebugDrawPointArray.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(), DebugDrawPointArray[i], DebugDrawPointArray[i + 1], FColor::Blue, false, 10, 0, 0);
	}
}