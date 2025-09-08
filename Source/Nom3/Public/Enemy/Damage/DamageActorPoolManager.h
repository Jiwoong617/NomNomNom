// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "GameFramework/Actor.h"
#include "DamageActorPoolManager.generated.h"

class ADamageActor;

UCLASS()
class NOM3_API ADamageActorPoolManager : public AActor
{
	GENERATED_BODY()

public:
	ADamageActorPoolManager();
	
protected:
	virtual void BeginPlay() override;
	
	TQueue<ADamageActor*> DamageActorPool;

public:
	UFUNCTION()
	ADamageActor* SpawnDamageActor();

	UFUNCTION()
	ADamageActor* PopDamageActorFromPool();

	UFUNCTION()
	void PushDamageActorToPool(ADamageActor* DamageActor);

	UFUNCTION()
	void ShowDamageActor(FVector Location);
};
