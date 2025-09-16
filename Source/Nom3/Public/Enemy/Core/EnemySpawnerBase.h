// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerBase.generated.h"

UCLASS()
class NOM3_API AEnemySpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawnerBase();

	UFUNCTION()
	virtual void OnDetectPlayerPawn();
};
