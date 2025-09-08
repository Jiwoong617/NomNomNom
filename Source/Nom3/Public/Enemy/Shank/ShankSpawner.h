// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShankSpawner.generated.h"

class UArrowComponent;
class AShankBase;

UCLASS()
class NOM3_API AShankSpawner : public AActor
{
	GENERATED_BODY()

public:
	AShankSpawner();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
	TObjectPtr<UArrowComponent> LaunchDirection;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AShankBase> ScoutShankClass;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AShankBase> SelfDestructShankClass;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float SpawnRate;

	UPROPERTY(EditAnywhere, Category="Spawning")
	int32 SpawnMin;

	UPROPERTY(EditAnywhere, Category="Spawning")
	int32 SpawnMax;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float LaunchConeAngle;

	UFUNCTION()
	void LaunchShanks(int32 Count);
};