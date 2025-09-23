// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/DroneSpawner.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/Common/DroneBase.h"

ADroneSpawner::ADroneSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//정찰 생크 로드
	if (static ConstructorHelpers::FClassFinder<ADroneBase>
		Finder(TEXT("/Game/Enemies/Shank/BP_ScoutShank.BP_ScoutShank_C"));
		Finder.Class)
	{
		EnemyList.Emplace(Finder.Class);
	}

	//자폭 생크 로드
	if (static ConstructorHelpers::FClassFinder<ADroneBase>
		Finder(TEXT("/Game/Enemies/Shank/BP_SelfDestructShank.BP_SelfDestructShank_C"));
		Finder.Class)
	{
		EnemyList.Emplace(Finder.Class);
	}
}

void ADroneSpawner::OnNoticePawn(AActor* DetectedPawn)
{
	Super::OnNoticePawn(DetectedPawn);

	//활성화
	ActiveAutoSpawn();
}

void ADroneSpawner::SpawnSpecific(const FTransform& SpawnTransform)
{
	//이번 에너미 클래스 획득
	const TSubclassOf<AEnemyActorBase> ThisOrder = StockList[StockIndex++];
	
	//드론 스폰
	if (AEnemyActorBase* Spawned = GetWorld()->SpawnActor<AEnemyActorBase>(ThisOrder, SpawnTransform))
	{
		if (const ADroneBase* SpawnedDrone = Cast<ADroneBase>(Spawned))
		{
			//사출
			SpawnedDrone->DroneMoveComp->Launch(SpawnTransform.GetRotation().GetForwardVector());
		}
	}
}