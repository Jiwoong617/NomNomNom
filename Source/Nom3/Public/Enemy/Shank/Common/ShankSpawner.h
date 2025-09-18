// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemySpawnerBase.h"
#include "GameFramework/Actor.h"
#include "ShankSpawner.generated.h"

//전방 선언
class AEnemyActorBase;
class APlayerDetectVolume;
class USphereComponent;
class UArrowComponent;
class AShankBase;

UCLASS()
class NOM3_API AShankSpawner : public AEnemySpawnerBase
{
	GENERATED_BODY()

public:
	AShankSpawner();

	virtual void OnDetectPlayerPawn() override;

protected:
	virtual void SpawnSpecific(const FTransform& SpawnTransform) override;
};