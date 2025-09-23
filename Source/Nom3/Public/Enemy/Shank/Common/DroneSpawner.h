// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemySpawnerBase.h"
#include "GameFramework/Actor.h"
#include "DroneSpawner.generated.h"

//전방 선언
class AEnemyActorBase;
class APlayerDetectVolume;
class USphereComponent;
class UArrowComponent;
class ADroneBase;

UCLASS()
class NOM3_API ADroneSpawner : public AEnemySpawnerBase
{
	GENERATED_BODY()

public:
	ADroneSpawner();

	virtual void OnNoticePawn(AActor* DetectedPawn) override;

protected:
	virtual void SpawnSpecific(const FTransform& SpawnTransform) override;
};