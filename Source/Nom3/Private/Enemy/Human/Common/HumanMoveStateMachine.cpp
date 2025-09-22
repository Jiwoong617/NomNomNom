// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanMoveStateMachine.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

UHumanMoveStateMachine::UHumanMoveStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHumanMoveStateMachine::EnterState()
{
	Super::EnterState();

	//목표 지점을 찾는다
	const FVector Forward = OwnerHuman->TargetPawn->GetActorForwardVector();
	const FVector Up = OwnerHuman->TargetPawn->GetActorUpVector();
	const FVector RandDir = Forward.RotateAngleAxis(FMath::FRandRange(-180.0, 180.0), Up);
	Destination = OwnerHuman->TargetPawn->GetActorLocation() + RandDir * 2000;

	//목표 지점 위치의 도달 가능한 위치
	OwnerHuman->FindReachableLocation(Destination);

	//이동 시도
	const EPathFollowingRequestResult::Type MoveResult = OwnerHuman->AIController->MoveToLocation(Destination, 25);

	//결과 확인
	if (MoveResult == EPathFollowingRequestResult::Failed || MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//목적지 주변의 도달 가능한 지점으로 업데이트
		OwnerHuman->FindReachableLocation(Destination);
	}

	//4초 후에 Move 상태로 전환할 예정
	LimitTimeInState = 4;
}

void UHumanMoveStateMachine::ExecuteState()
{
	Super::ExecuteState();

	//목표 방향
	const FVector TargetDir = (OwnerHuman->TargetPawn->GetActorLocation() - OwnerHuman->GetActorLocation()).GetSafeNormal();
	const FRotator TargetRot = UKismetMathLibrary::MakeRotFromXZ(TargetDir, OwnerHuman->GetActorUpVector());
	OwnerHuman->MeshSceneComp->SetWorldRotation(TargetRot);

	if (ElapsedTimeInState > LimitTimeInState)
	{
		//이동 상태 머신으로 전환
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->MoveStateMachine);
	}
}

void UHumanMoveStateMachine::ExitState()
{
	Super::ExitState();
}