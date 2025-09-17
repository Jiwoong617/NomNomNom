// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerBase.generated.h"

<<<<<<< Updated upstream
=======
class UArrowComponent;
class APlayerDetectVolume;
class AEnemyActorBase;

>>>>>>> Stashed changes
UCLASS()
class NOM3_API AEnemySpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawnerBase();

	UFUNCTION()
	virtual void OnDetectPlayerPawn();
<<<<<<< Updated upstream
=======

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
	TObjectPtr<UArrowComponent> SpawnFrustumDirection;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float SpawnFrustumAngle;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnFrustumMin;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnFrustumMax;

	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ToolTip="이 스포너가 소환할 수 있는 재고의 숫자. StockList에 직접 할당된 요소의 개수가 NumOfStock에 못 미친다면 랜덤으로 할당된다."))
	int32 NumOfStock;

	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ToolTip="랜덤으로 선택될 수 있는 에너미 클래스 리스트"))
	TArray<TSubclassOf<AEnemyActorBase>> EnemyList;

	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ToolTip="이 스포너가 순서내도 소환할 에너미 클래스 배열. 직접 설정 가능하나 NumOfStock이 채워질 때까지 랜덤 요소로 채워진다."))
	TArray<TSubclassOf<AEnemyActorBase>> StockList;

	UPROPERTY(VisibleAnywhere)
	int32 SpawnCounter;

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
	FTransform GetSpawnTransform() const;

	UFUNCTION()
	void RequestSpawnEnemies(int32 Count);

	UFUNCTION()
	void ProcessSpawn();

	UFUNCTION()
	virtual void SpawnSpecific(const FTransform& SpawnTransform);
>>>>>>> Stashed changes
};
