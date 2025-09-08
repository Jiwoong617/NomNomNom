// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Damage/DamageActorPoolManager.h"

#include "Enemy/Damage/DamageActor.h"

ADamageActorPoolManager::ADamageActorPoolManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADamageActorPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

ADamageActor* ADamageActorPoolManager::SpawnDamageActor()
{
	//데미지 액터 스폰
	const auto Spawned = GetWorld()->SpawnActor<ADamageActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	DamageActorPool.Enqueue(Spawned);
	return Spawned;
}

ADamageActor* ADamageActorPoolManager::PopDamageActorFromPool()
{
	//데미지 액터 풀링 성공
	if (ADamageActor* Pop; DamageActorPool.Dequeue(Pop))
	{
		//반환
		return Pop;
	}

	//새로 데미지 액터를 스폰해서 반환
	return GetWorld()->SpawnActor<ADamageActor>(FVector::ZeroVector, FRotator::ZeroRotator);
}

void ADamageActorPoolManager::PushDamageActorToPool(ADamageActor* DamageActor)
{
	//데미지 액터 큐에 삽입
	DamageActorPool.Enqueue(DamageActor);
}

void ADamageActorPoolManager::ShowDamageActor(FVector Location)
{
	//사용할 수 있는 데미지 액터 획득
	auto Temp = PopDamageActorFromPool();

	Temp->ShowAndHide();
}
