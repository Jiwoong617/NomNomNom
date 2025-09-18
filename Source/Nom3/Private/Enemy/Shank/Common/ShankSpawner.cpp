// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/ShankSpawner.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/Common/ShankBase.h"

AShankSpawner::AShankSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//정찰 생크 소환
	if (static ConstructorHelpers::FClassFinder<AShankBase>
		Finder(TEXT("/Game/Enemies/Shank/BP_ScoutShank.BP_ScoutShank_C"));
		Finder.Class)
	{
		EnemyList.Emplace(Finder.Class);
	}

	//자폭 생크 소환
	if (static ConstructorHelpers::FClassFinder<AShankBase>
		Finder(TEXT("/Game/Enemies/Shank/BP_SelfDestructShank.BP_SelfDestructShank_C"));
		Finder.Class)
	{
		EnemyList.Emplace(Finder.Class);
	}
}

void AShankSpawner::OnDetectPlayerPawn()
{
	//최초 생크 사출
	RequestSpawnEnemies(FMath::RandRange(SpawnMin, SpawnMax));

	//2초마다 주기적으로 생크 사출
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		RequestSpawnEnemies(FMath::RandRange(SpawnMin, SpawnMax));
	}, SpawnRate, true);
}

void AShankSpawner::SpawnSpecific(const FTransform& SpawnTransform)
{
	Super::SpawnSpecific(SpawnTransform);

	//이번 에너미 클래스 획득
	const TSubclassOf<AEnemyActorBase> ThisOrder = StockList[SpawnCounter++];
	
	//생크 스폰
	if (AEnemyActorBase* Spawned = GetWorld()->SpawnActor<AEnemyActorBase>(ThisOrder, SpawnTransform))
	{
		if (const AShankBase* SpawnedShank = Cast<AShankBase>(Spawned))
		{
			//사출
			SpawnedShank->DroneMoveComp->Launch(SpawnTransform.GetRotation().GetForwardVector());
		}
	}
}