// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/SelfDestructShank/SelfDestructFindPathStateMachine.h"
#include "Enemy/Shank/Common/ShankBase.h"

USelfDestructFindPathStateMachine::USelfDestructFindPathStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USelfDestructFindPathStateMachine::EnterState()
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

void USelfDestructFindPathStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (OwnerShank == false)
	{
		return;
	}

	//제한 시간 초과
	if (ElapsedTimeInState > LimitTimeInState)
	{
		OwnerShank->ChangeCurrentStateMachine(OwnerShank->FollowPathStateMachine);
	}
}

void USelfDestructFindPathStateMachine::ExitState()
{
	Super::ExitState();
	
	if (OwnerShank == false)
	{
		return;
	}
}

void USelfDestructFindPathStateMachine::DecideTargetLocation() const
{
	//목표의 방향 요소 벡터
	const FVector UpDir = OwnerShank->TargetPawn->GetActorUpVector();

	//목표의 2m 상공을 목표 위치로 잡는다
	OwnerShank->Destination = OwnerShank->TargetPawn->GetActorLocation() + UpDir * 200;
}

void USelfDestructFindPathStateMachine::CalculatePaths() const
{
	//경로 배열을 비운다
	OwnerShank->PathQueue.Empty();

	//시작 위치와 목표 위치
	const FVector StartLocation = OwnerShank->GetActorLocation();
	const FVector TargetLocation = OwnerShank->Destination;

	//시선 방향에 우측 벡터
	const FVector GazeRightDir = OwnerShank->GetPlayerGazeRightDir();

	//세그먼트 개수
	constexpr int32 NumSegments = 3;

	//디버그 드로우를 위한 배열
	TArray<FVector> DebugDrawPointArray;

	//경로 큐에 입력
	for (int32 i = 1; i < NumSegments; ++i)
	{
		//지그재그 규모
		const float RandomOffset = FMath::RandRange(-200, 200);

		//세그먼트 중간점
		const float Alpha = static_cast<float>(i) / NumSegments;
		FVector IntermediatePoint = FMath::Lerp(StartLocation, TargetLocation, Alpha);

		//랜덤 포인트 획득
		IntermediatePoint += GazeRightDir * RandomOffset;

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