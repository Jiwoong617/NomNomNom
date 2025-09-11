// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/DamageActorPoolGameInstanceSubsystem.h"
#include "Enemy/Damage/DamageActor.h"
#include "Nom3/Nom3.h"

UDamageActorPoolGameInstanceSubsystem::UDamageActorPoolGameInstanceSubsystem()
{
	//데미지 액터 클래스 로드
	if (ConstructorHelpers::FClassFinder<ADamageActor> Finder(TEXT("/Game/Enemies/Damage/BP_NormalLeftDamageActor.BP_NormalLeftDamageActor_C"));
		Finder.Succeeded())
	{
		NormalLeftDamageActorClass = Finder.Class;	
	}

	//데미지 액터 클래스 로드
	if (ConstructorHelpers::FClassFinder<ADamageActor> Finder(TEXT("/Game/Enemies/Damage/BP_NormalRightDamageActor.BP_NormalRightDamageActor_C"));
		Finder.Succeeded())
	{
		NormalRightDamageActorClass = Finder.Class;	
	}

	//데미지 액터 클래스 로드
	if (ConstructorHelpers::FClassFinder<ADamageActor> Finder(TEXT("/Game/Enemies/Damage/BP_CriticalLeftDamageActor.BP_CriticalLeftDamageActor_C"));
		Finder.Succeeded())
	{
		CriticalLeftDamageActorClass = Finder.Class;	
	}

	//데미지 액터 클래스 로드
	if (ConstructorHelpers::FClassFinder<ADamageActor> Finder(TEXT("/Game/Enemies/Damage/BP_CriticalRightDamageActor.BP_CriticalRightDamageActor_C"));
		Finder.Succeeded())
	{
		CriticalRightDamageActorClass = Finder.Class;	
	}
}

void UDamageActorPoolGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//각각 15개의 데미지 액터를 풀에 저장
	for (int32 i = 0; i < 15; i++)
	{
		auto Spawned = SpawnNormalDamageActor();
		NormalDamageActorPool.Enqueue(Spawned);
	}
	for (int32 i = 0; i < 15; i++)
	{
		auto Spawned = SpawnCriticalDamageActor();
		CriticalDamageActorPool.Enqueue(Spawned);
	}
}

ADamageActor* UDamageActorPoolGameInstanceSubsystem::SpawnNormalDamageActor() const
{
	//데미지 액터 스폰
	const auto Selected = FMath::RandRange(0, 1) ? NormalLeftDamageActorClass : NormalRightDamageActorClass;
	const auto Spawned = GetWorld()->SpawnActor<ADamageActor>(Selected, FVector::ZeroVector, FRotator::ZeroRotator);
	return Spawned;
}

ADamageActor* UDamageActorPoolGameInstanceSubsystem::SpawnCriticalDamageActor() const
{
	//데미지 액터 스폰
	const auto Selected = FMath::RandRange(0, 1) ? CriticalLeftDamageActorClass : CriticalRightDamageActorClass;
	const auto Spawned = GetWorld()->SpawnActor<ADamageActor>(Selected, FVector::ZeroVector, FRotator::ZeroRotator);
	return Spawned;
}

ADamageActor* UDamageActorPoolGameInstanceSubsystem::PopNormalDamageActorFromPool()
{
	//데미지 액터 풀링 성공
	if (ADamageActor* Pop; NormalDamageActorPool.Dequeue(Pop))
	{
		//반환
		return Pop;
	}

	//새로 데미지 액터를 스폰해서 반환
	return SpawnNormalDamageActor();
}

ADamageActor* UDamageActorPoolGameInstanceSubsystem::PopCriticalDamageActorFromPool()
{
	//데미지 액터 풀링 성공
	if (ADamageActor* Pop; CriticalDamageActorPool.Dequeue(Pop))
	{
		//반환
		return Pop;
	}

	//새로 데미지 액터를 스폰해서 반환
	return SpawnCriticalDamageActor();
}

void UDamageActorPoolGameInstanceSubsystem::PushDamageActorToPool(ADamageActor* DamageActor)
{
	const auto Type = DamageActor->GetClass();
	if (Type == NormalLeftDamageActorClass || Type == NormalRightDamageActorClass)
	{
		//일반 데미지 액터 풀에 삽입
		NormalDamageActorPool.Enqueue(DamageActor);
	}
	if (Type == CriticalLeftDamageActorClass || Type == CriticalRightDamageActorClass)
	{
		//크리티컬 데미지 액터 풀에 삽입
		CriticalDamageActorPool.Enqueue(DamageActor);
	}
}

void UDamageActorPoolGameInstanceSubsystem::ShowNormalDamageActor(const FVector& Location, const int32 Damage)
{
	//사용할 수 있는 데미지 액터 획득
	const auto Pop = PopNormalDamageActorFromPool();

	//목표 위치로 이동
	Pop->SetActorLocation(Location);

	//데미지 액터 활성화
	Pop->DisplayDamage(Damage);
}

void UDamageActorPoolGameInstanceSubsystem::ShowCriticalDamageActor(const FVector& Location, const int32 Damage)
{
	//사용할 수 있는 데미지 액터 획득
	const auto Pop = PopCriticalDamageActorFromPool();

	//목표 위치로 이동
	Pop->SetActorLocation(Location);

	//데미지 액터 활성화
	Pop->DisplayDamage(Damage);
}