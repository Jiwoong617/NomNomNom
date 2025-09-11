// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankSpawner.h"
#include "Components/ArrowComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ShankBase.h"
#include "Kismet/KismetMathLibrary.h"

AShankSpawner::AShankSpawner() : SpawnMin(1), SpawnMax(1), LaunchConeAngle(15)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스폰된 생크들이 사출되는 방향을 가리키는 애로우 컴포넌트
	LaunchDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnDirection"));
	LaunchDirection->ArrowSize = 5;
	LaunchDirection->SetupAttachment(GetRootComponent());
}

void AShankSpawner::BeginPlay()
{
	Super::BeginPlay();

	LaunchShanks(FMath::RandRange(SpawnMin, SpawnMax));

	//2초마다 주기적으로 생크 스폰
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		LaunchShanks(FMath::RandRange(SpawnMin, SpawnMax));
	}, SpawnRate, true);
}

void AShankSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShankSpawner::LaunchShanks(const int32 Count)
{
	//유효하지 않은 요청
	if (Count < 1)
	{
		return;
	}
	
	//이전 요청이 아직 마무리 되지 않았다면
	if (SpawnTimerHandle.IsValid())
	{
		return;
	}
	
	//이전 요청을 처리할 때까지 추가 요청을 차단
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this]()
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}, Count * 0.25, false);

	//생크 소환 람다식
	auto SpawnLambda = [this]()
	{
		//원뿔 범위로 변환
		const FVector ConeDirection = LaunchDirection->GetForwardVector();
		const FVector RandomDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ConeDirection, LaunchConeAngle);
		const FVector SpawnLocation = LaunchDirection->GetComponentLocation();
		const FRotator SpawnRotation = RandomDirection.Rotation();
		const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		
		//정찰 생크 스폰
		if (const AShankBase* SpawnShank = GetWorld()->SpawnActor<AShankBase>(ScoutShankClass, SpawnTransform);
			SpawnShank && SpawnShank->DroneMoveComp)
		{
			//사출
			SpawnShank->DroneMoveComp->Launch(RandomDirection);
		}
	};

	//생크 1회 스폰
	SpawnLambda();
	
	//요청한 만큼 생크를 반복 스폰
	for (int32 i = 0; i < Count; i++)
	{
		FTimerHandle Temp;
		GetWorldTimerManager().SetTimer(Temp, SpawnLambda, i * 0.25, false);	
	}
}