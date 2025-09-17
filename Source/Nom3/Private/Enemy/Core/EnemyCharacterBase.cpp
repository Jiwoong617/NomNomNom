// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyCharacterBase.h"

#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacterBase::BeginPlay()
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

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}
}

UStateMachineBase* AEnemyCharacterBase::GetCurrentStateMachine() const
{
	return CurrentStateMachine;
}

void AEnemyCharacterBase::ChangeCurrentStateMachine(UStateMachineBase* StateMachineToChange)
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

void AEnemyCharacterBase::OnAimByPlayerSight()
{
	
}

void AEnemyCharacterBase::OnDamaged(FFireInfo Info)
{
	
}

void AEnemyCharacterBase::OnCriticalDamaged(FFireInfo Info)
{
	
}

FVector AEnemyCharacterBase::GetPlayerGazeDir() const
{
	return (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();	
}

FVector AEnemyCharacterBase::GetPlayerGazeUpDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	const FVector Element = FVector::DotProduct(PlayerGazeDir, TargetPawn->GetActorUpVector()) * PlayerGazeDir;
	return (TargetPawn->GetActorUpVector() - Element).GetSafeNormal();
}

FVector AEnemyCharacterBase::GetPlayerGazeRightDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	return FVector::CrossProduct(TargetPawn->GetActorUpVector(), PlayerGazeDir).GetSafeNormal();
}