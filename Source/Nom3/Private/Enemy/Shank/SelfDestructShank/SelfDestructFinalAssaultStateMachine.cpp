// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/SelfDestructShank/SelfDestructFinalAssaultStateMachine.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Shank/Common/ShankBase.h"

USelfDestructFinalAssaultStateMachine::USelfDestructFinalAssaultStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USelfDestructFinalAssaultStateMachine::EnterState()
{
	Super::EnterState();

	if (OwnerShank == false)
	{
		return;
	}
}

void USelfDestructFinalAssaultStateMachine::ExecuteState()
{
	Super::ExecuteState();

	if (OwnerShank == false)
	{
		return;
	}

	//목표를 향해 돌진한다
	OwnerShank->Destination = OwnerShank->TargetPawn->GetActorLocation();

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("What the Fuck!?"));

	//목표와의 거리에 따라
	if (const FVector TargetDiff = OwnerShank->TargetPawn->GetActorLocation() - OwnerShank->GetActorLocation();
		TargetDiff.Length() < 750)
	{
		//체력이 0이되면 자연스럽게 자폭하며 아무런 상태 머신도 실행되지 않는다
		OwnerShank->HealthComp->HP = 0;
	}
	else if (TargetDiff.Length() >= 1500)
	{
		//목표에게 도달할 수 있는 새로운 경로를 찾아낸다
		OwnerShank->ChangeCurrentStateMachine(OwnerShank->FindPathStateMachine);
	}
}

void USelfDestructFinalAssaultStateMachine::ExitState()
{
	Super::ExitState();

	if (OwnerShank == false)
	{
		return;
	}
}