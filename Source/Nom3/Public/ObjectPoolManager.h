// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolManager.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UObjectPoolable : public UInterface
{
	GENERATED_BODY()
};

class IObjectPoolable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ObjectPoolable")
	void OnPushed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ObjectPoolable")
	void OnPopped();
};

UCLASS()
class NOM3_API UObjectPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	void CreateObjectPool(TSubclassOf<AActor> PooledObjectClass, int32 PoolSize);
	
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	AActor* PopObject(TSubclassOf<AActor> PooledObjectClass);
	
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	void PushObject(AActor* PooledObject);

private:
	TMap<UClass*, TArray<AActor*>> ObjectPools;
	TMap<UClass*, TArray<AActor*>> ActiveObjects;
};
