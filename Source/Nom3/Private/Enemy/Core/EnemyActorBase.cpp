// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyActorBase.h"

#include "Enemy/Core/StateMachineBase.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"

AEnemyActorBase::AEnemyActorBase() :
	Health(100)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

<<<<<<< Updated upstream:Source/Nom3/Private/Enemy/Core/EnemyBase.cpp
void AEnemyBase::OnAimByPlayerSight()
=======
void AEnemyActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	//플레어어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//데미지 액터 풀링 서브시스템 획득
	if (auto Temp = GetWorld()->GetSubsystem<UDamageActorPoolWorldSubsystem>())
	{
		DamageActorPool = Temp;
	}
}

void AEnemyActorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}
}

UStateMachineBase* AEnemyActorBase::GetCurrentStateMachine() const
{
	return CurrentStateMachine;
}

void AEnemyActorBase::ChangeCurrentStateMachine(UStateMachineBase* StateMachineToChange)
{
	//임시 저장
	const auto Before = CurrentStateMachine;

	//스테이트 머신 변경
	CurrentStateMachine = StateMachineToChange;

	//탈출
	if (Before)
	{
		Before->ExitState();	
	}
}

void AEnemyActorBase::OnAimByPlayerSight()
>>>>>>> Stashed changes:Source/Nom3/Private/Enemy/Core/EnemyActorBase.cpp
{
	
}

void AEnemyActorBase::OnDamaged(FFireInfo Info)
{
	
}

void AEnemyActorBase::OnCriticalDamaged(FFireInfo Info)
{
	
}

<<<<<<< Updated upstream:Source/Nom3/Private/Enemy/Core/EnemyBase.cpp
void AEnemyBase::BeginPlay()
=======
FVector AEnemyActorBase::GetPlayerGazeDir() const
>>>>>>> Stashed changes:Source/Nom3/Private/Enemy/Core/EnemyActorBase.cpp
{
	Super::BeginPlay();
	
	//플레어어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

<<<<<<< Updated upstream:Source/Nom3/Private/Enemy/Core/EnemyBase.cpp
	//데미지 액터 풀링 서브시스템 획득
	if (auto Temp = GetWorld()->GetSubsystem<UDamageActorPoolWorldSubsystem>())
	{
		DamageActorPool = Temp;
	}
=======
FVector AEnemyActorBase::GetPlayerGazeUpDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	const FVector Element = FVector::DotProduct(PlayerGazeDir, TargetPawn->GetActorUpVector()) * PlayerGazeDir;
	return (TargetPawn->GetActorUpVector() - Element).GetSafeNormal();
}

FVector AEnemyActorBase::GetPlayerGazeRightDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	return FVector::CrossProduct(TargetPawn->GetActorUpVector(), PlayerGazeDir).GetSafeNormal();
>>>>>>> Stashed changes:Source/Nom3/Private/Enemy/Core/EnemyActorBase.cpp
}