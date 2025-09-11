// Copyright Epic Games, Inc. All Rights Reserved.

#include "ObjectPoolManager.h"
#include "Engine/World.h"

void UObjectPoolManager::CreateObjectPool(TSubclassOf<AActor> PooledObjectClass, int32 PoolSize)
{
	if (!PooledObjectClass)
	{
		return;
	}

	TArray<AActor*>& Pool = ObjectPools.FindOrAdd(PooledObjectClass);

	for (int32 i = 0; i < PoolSize; ++i)
	{
		AActor* PooledObject = GetWorld()->SpawnActor<AActor>(PooledObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (PooledObject)
		{
			PooledObject->SetActorEnableCollision(false);
			PooledObject->SetActorHiddenInGame(true);
			PooledObject->SetActorTickEnabled(false);
			Pool.Add(PooledObject);

			if (PooledObject->GetClass()->ImplementsInterface(UObjectPoolable::StaticClass()))
			{
				IObjectPoolable::Execute_OnPushed(PooledObject);
			}
		}
	}
}

AActor* UObjectPoolManager::PopObject(TSubclassOf<AActor> PooledObjectClass)
{
	TArray<AActor*>* Pool = ObjectPools.Find(PooledObjectClass);
	AActor* PooledObject = nullptr;

	if (Pool && Pool->Num() > 0)
	{
		PooledObject = Pool->Pop();
	}
	else
	{
		PooledObject = GetWorld()->SpawnActor<AActor>(PooledObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (PooledObject)
	{
		PooledObject->SetActorEnableCollision(true);
			PooledObject->SetActorHiddenInGame(false);
			PooledObject->SetActorTickEnabled(true);
		ActiveObjects.FindOrAdd(PooledObjectClass).Add(PooledObject);

		if (PooledObject->GetClass()->ImplementsInterface(UObjectPoolable::StaticClass()))
		{
			IObjectPoolable::Execute_OnPopped(PooledObject);
		}
	}

	return PooledObject;
}

void UObjectPoolManager::PushObject(AActor* PooledObject)
{
	if (!PooledObject)
	{
		return;
	}

	UClass* PooledObjectClass = PooledObject->GetClass();
	TArray<AActor*>* ActivePool = ActiveObjects.Find(PooledObjectClass);
	if(ActivePool)
	{
		ActivePool->Remove(PooledObject);
	}

	PooledObject->SetActorEnableCollision(false);
	PooledObject->SetActorHiddenInGame(true);
	PooledObject->SetActorTickEnabled(false);

	ObjectPools.FindOrAdd(PooledObjectClass).Add(PooledObject);

	if (PooledObject->GetClass()->ImplementsInterface(UObjectPoolable::StaticClass()))
	{
		IObjectPoolable::Execute_OnPushed(PooledObject);
	}
}