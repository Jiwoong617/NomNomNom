// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanEvadeStateMachine.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UHumanEvadeStateMachine::UHumanEvadeStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHumanEvadeStateMachine::EnterState()
{
	Super::EnterState();

	//최초 이동 속도 저장
	InitWalkSpeed = OwnerHuman->GetCharacterMovement()->MaxWalkSpeed;

	//이동 속도 2배로 상승
	OwnerHuman->GetCharacterMovement()->MaxWalkSpeed = InitWalkSpeed * 2;

	//시작점 설정
	const FVector Start = OwnerHuman->GetActorLocation();

	//플레이어의 시선에서 좌우 방향
	const FVector RightDir = OwnerHuman->GetPlayerGazeRightDir();

	//우측 람다식
	auto Lambda1 = [this, Start, RightDir]()
	{
		for (int i = 5; i >= 3; i++)
		{
			if (FVector Find = Start + RightDir * i * 100; OwnerHuman->FindReachableLocation(Find))
			{
				Destination = Find;
				return true;
			}
		}

		return false;
	};

	//우측 람다식
	auto Lambda2 = [this, Start, RightDir]()
	{
		for (int i = 5; i >= 3; i++)
		{
			if (FVector Find = Start - RightDir * i * 100; OwnerHuman->FindReachableLocation(Find))
			{
				Destination = Find;
				return true;
			}
		}

		return false;
	};

	//왼쪽 혹은 오른쪽으로 회피 운동
	if (FMath::RandRange(0, 1))
	{
		if (Lambda1())
		{
			OwnerHuman->AIController->MoveToLocation(Destination, 50);	
		}

		if (Lambda2())
		{
			OwnerHuman->AIController->MoveToLocation(Destination, 50);	
		}
	}
	else
	{
		if (Lambda2())
		{
			OwnerHuman->AIController->MoveToLocation(Destination, 50);	
		}
		
		if (Lambda1())
		{
			OwnerHuman->AIController->MoveToLocation(Destination, 50);	
		}
	}
}

void UHumanEvadeStateMachine::ExecuteState()
{
	Super::ExecuteState();

	//이동 시도
	const EPathFollowingRequestResult::Type MoveResult = OwnerHuman->AIController->MoveToLocation(Destination, 25);

	//결과 확인
	if (MoveResult == EPathFollowingRequestResult::Failed || MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//목적지 주변의 도달 가능한 지점으로 업데이트
		OwnerHuman->FindReachableLocation(Destination);
	}

	//목표 도달
	if (const FVector Diff = OwnerHuman->GetActorLocation() - Destination; Diff.Length() < 50)
	{
		//목표 위치에 도달했으므로 대기 상태로 전환
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->IdleStateMachine);
	}

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, TEXT("UHumanEvadeStateMachine::ExecuteState"));
}

void UHumanEvadeStateMachine::ExitState()
{
	Super::ExitState();

	//기본 이동 속도로 복귀
	OwnerHuman->GetCharacterMovement()->MaxWalkSpeed = InitWalkSpeed;
}