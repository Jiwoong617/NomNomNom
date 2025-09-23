// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanEvadeStateMachine.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

	//회피 위치 검색 시도
	if (FindEvadeLocation(EvadeLocation))
	{
		//최초 이동 속도 저장
		InitWalkSpeed = OwnerHuman->GetCharacterMovement()->MaxWalkSpeed;

		//이동 속도 2배로 상승
		OwnerHuman->GetCharacterMovement()->MaxWalkSpeed = InitWalkSpeed * 5;
		
		//이동 시도
		const EPathFollowingRequestResult::Type MoveResult = OwnerHuman->AIController->MoveToLocation(EvadeLocation, 25);

		//애니메이션 몽타주 재생
		OwnerHuman->GetMesh()->GetAnimInstance()->Montage_Play(EvadeAnimMontage);

		//탈출 시간 설정
		LimitTimeInState = 2;
	}
	else
	{
		//대기 상태로 전환
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->IdleStateMachine);
	}
}

void UHumanEvadeStateMachine::ExecuteState()
{
	Super::ExecuteState();

	//목표 방향
	const FVector TargetDir = OwnerHuman->GetCharacterMovement()->Velocity.GetSafeNormal();
	const FRotator TargetRot = UKismetMathLibrary::MakeRotFromXZ(TargetDir, OwnerHuman->GetActorUpVector());
	OwnerHuman->MeshSceneComp->SetWorldRotation(TargetRot);

	if (ElapsedTimeInState > LimitTimeInState)
	{
		//이동 상태 머신으로 전환
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->MoveStateMachine);
	}
}

void UHumanEvadeStateMachine::ExitState()
{
	Super::ExitState();

	//기본 이동 속도로 복귀
	OwnerHuman->GetCharacterMovement()->MaxWalkSpeed = InitWalkSpeed;
}

bool UHumanEvadeStateMachine::FindEvadeLocation(FVector& OutLocation)
{
	//시작점 설정
	const FVector Start = OwnerHuman->GetActorLocation();

	//플레이어의 시선에서 좌우 방향
	const FVector RightDir = OwnerHuman->GetPlayerGazeRightDir();

	//우측 확인
	for (int i = 13; i >= 10; i--)
	{
		FVector Find = Start + RightDir * i * 100;
		//(GetWorld(), Start, Find, FColor::Red, false, 5);
		if (OwnerHuman->FindReachableLocation(Find))
		{
			EvadeLocation = Find;
			return true;
		}
	}

	//좌측 확인
	for (int i = 13; i >= 10; i--)
	{
		FVector Find = Start - RightDir * i * 100;
		
		//DrawDebugLine(GetWorld(), Start, Find, FColor::Red, false, 5);
		if (OwnerHuman->FindReachableLocation(Find))
		{
			EvadeLocation = Find;
			return true;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Fail To Find Evade Location!"));

	//실패
	return false;
}