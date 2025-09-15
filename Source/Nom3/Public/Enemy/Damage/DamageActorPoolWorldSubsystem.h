// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "DamageActorPoolWorldSubsystem.generated.h"

class ADamageActor;

UCLASS()
class NOM3_API UDamageActorPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UDamageActorPoolWorldSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
protected:
	TQueue<ADamageActor*> NormalDamageActorPool;

	TQueue<ADamageActor*> CriticalDamageActorPool;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ADamageActor> NormalLeftDamageActorClass;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ADamageActor> NormalRightDamageActorClass;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ADamageActor> CriticalLeftDamageActorClass;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ADamageActor> CriticalRightDamageActorClass;

	UFUNCTION()
	ADamageActor* SpawnNormalDamageActor() const;

	UFUNCTION()
	ADamageActor* SpawnCriticalDamageActor() const;

	UFUNCTION()
	ADamageActor* PopNormalDamageActorFromPool();

	UFUNCTION()
	ADamageActor* PopCriticalDamageActorFromPool();
	
public:
	UFUNCTION()
	void PushDamageActorToPool(ADamageActor* DamageActor);

	UFUNCTION()
	void ShowNormalDamageActor(const FVector& Location, int32 Damage);

	UFUNCTION()
	void ShowCriticalDamageActor(const FVector& Location, int32 Damage);
};
