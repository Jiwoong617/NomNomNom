// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemySpawnerBase.h"
#include "GameFramework/Actor.h"
#include "ShankSpawner.generated.h"

//전방 선언
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

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 NumOfStock;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
	TObjectPtr<UArrowComponent> LaunchDirection;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AShankBase> ScoutShankClass;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AShankBase> SelfDestructShankClass;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TObjectPtr<APlayerDetectVolume> BindPlayerDetectVolume;
	
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float SpawnRate;

	UPROPERTY(EditAnywhere, Category="Spawning")
	int32 SpawnMin;

	UPROPERTY(EditAnywhere, Category="Spawning")
	int32 SpawnMax;

	UFUNCTION()
	void SpawnShank();

	UPROPERTY(EditAnywhere, Category="Spawning")
	float LaunchConeAngle;

	UFUNCTION()
	void LaunchShanks(int32 Count);
};