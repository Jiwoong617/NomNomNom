// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyCharacterBase.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//배치되어 있거나 스폰되면 AI에 의해 점유
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//원하는 AI 컨트롤러 스폰
	SpawnDefaultController();

	//AI 컨트롤러 획득
	AIController = Cast<AAIController>(GetController());

	//네비게이션 시스템 획득
	NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	//플레이어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void AEnemyCharacterBase::OnDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	//일반적인 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowNormalDamageActor(GetActorLocation(), Damage);
}

void AEnemyCharacterBase::OnCriticalDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	//크리티컬 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage * 2);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowCriticalDamageActor(GetActorLocation(), Damage);
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