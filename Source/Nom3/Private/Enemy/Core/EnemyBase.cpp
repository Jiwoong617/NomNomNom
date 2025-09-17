// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyBase::AEnemyBase() :
	Health(100)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
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

void AEnemyBase::OnAimByPlayerSight()
{
	
}

void AEnemyBase::OnDamaged(FFireInfo Info)
{
	
}

void AEnemyBase::OnCriticalDamaged(FFireInfo Info)
{
	
}

FVector AEnemyBase::GetPlayerGazeDir() const
{
	return (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
}

FVector AEnemyBase::GetPlayerGazeUpDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	const FVector Element = FVector::DotProduct(PlayerGazeDir, TargetPawn->GetActorUpVector()) * PlayerGazeDir;
	return (TargetPawn->GetActorUpVector() - Element).GetSafeNormal();
}

FVector AEnemyBase::GetPlayerGazeRightDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	return FVector::CrossProduct(TargetPawn->GetActorUpVector(), PlayerGazeDir).GetSafeNormal();
}
