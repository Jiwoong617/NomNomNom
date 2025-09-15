// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerDetectVolume.generated.h"

class USphereComponent;
class AEnemySpawnerBase;

UCLASS()
class NOM3_API APlayerDetectVolume : public AActor
{
	GENERATED_BODY()

public:
	APlayerDetectVolume();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemySpawnerBase> BindSpawner;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PlayerDetectSphereComp;

	UPROPERTY(VisibleAnywhere)
	bool bActivate;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
