// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Human/Common/HumanController.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "Enemy/Human/Common/HumanStateMachineBase.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AHumanController::AHumanController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHumanController::OnMoveTaskCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess() || Result.IsFailure())
	{
		OwnerHuman->ChangeCurrentStateMachine(OwnerHuman->IdleStateMachine);
	}
}

// Called when the game starts or when spawned
void AHumanController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, GetPawn()->GetActorLabel());

	OwnerHuman = Cast<AHumanBase>(GetPawn());

	if (OwnerHuman)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, TEXT("AI Human Controller Success!"));
	}

	// AI의 PathFollowingComponent가 이동 요청을 끝냈을 때
	// OnMoveTaskCompleted 함수를 호출하도록 등록(바인딩)합니다.
	if (UPathFollowingComponent* PathComp = GetPathFollowingComponent())
	{
		PathComp->OnRequestFinished.AddUObject(this, &AHumanController::OnMoveTaskCompleted);
	}
}