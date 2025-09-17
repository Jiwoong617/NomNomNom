// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemySpawnerBase.h"

#include "Components/ArrowComponent.h"
#include "Enemy/Core/EnemyBase.h"
#include "Enemy/Core/PlayerDetectVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nom3/Nom3.h"

AEnemySpawnerBase::AEnemySpawnerBase() :
	SpawnFrustumAngle(15),
	SpawnFrustumMin(50),
	SpawnFrustumMax(100),
	NumOfStock(8),
	SpawnMin(4),
	SpawnMax(4)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//스폰된 생크들이 사출되는 방향을 가리키는 애로우 컴포넌트
	SpawnFrustumDirection = CreateDefaultSubobject<UArrowComponent>(FName("SpawnFrustumDirection"));
	SpawnFrustumDirection->ArrowSize = 5;
	SpawnFrustumDirection->SetupAttachment(GetRootComponent());
}

void AEnemySpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	//스폰 카운터
	SpawnCounter = 0;

	//비어 있는 만큼 채워 넣기
	for (int32 i = StockList.Num(); i < NumOfStock; i++)
	{
		TSubclassOf<AEnemyBase> Selected = nullptr;

		//메뉴가 1개라도 있다면
		if (EnemyList.Num() > 0)
		{
			//랜덤으로 하나 뽑는다
			Selected = EnemyList[FMath::RandRange(0, EnemyList.Num() - 1)];
		}

		//유효한 메뉴를 뽑았다면
		if (Selected)
		{
			StockList.Add(Selected);	
		}
	}
}

void AEnemySpawnerBase::OnDetectPlayerPawn()
{
	
}

void AEnemySpawnerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BindPlayerDetectVolume == nullptr)
	{
		BindPlayerDetectVolume = GetWorld()->SpawnActor<APlayerDetectVolume>();
		BindPlayerDetectVolume->SetActorLocation(GetActorLocation());
		BindPlayerDetectVolume->BindSpawner = this;
	}
}

FTransform AEnemySpawnerBase::GetSpawnTransform() const
{
	//절두체 범위 내 랜덤 방향으로 변환
	const FVector FrustumDirection = SpawnFrustumDirection->GetForwardVector();
	const FVector RandomDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(FrustumDirection, SpawnFrustumAngle);

	//절두체 범위 내 랜덤 위치 결정
	const float Length = FMath::FRandRange(SpawnFrustumMin, SpawnFrustumMax);
	const FVector SpawnLoc = SpawnFrustumDirection->GetComponentLocation() + RandomDirection * Length;

	//절두체 범위 내 랜덤 회전 결정
	const FRotator SpawnRot = RandomDirection.Rotation();

	//트랜스폼 구성
	const FTransform SpawnTransform(SpawnRot, SpawnLoc);

	//반환
	return SpawnTransform;
}

void AEnemySpawnerBase::RequestSpawnEnemies(const int32 Count)
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

	//델리게이트 바인딩
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, FName("ProcessSpawn"));
	
	//생크 1회 스폰
	Delegate.Execute();
	
	//요청한 만큼 생크를 반복 스폰
	for (int32 i = 0; i < Count; i++)
	{
		FTimerHandle Temp;
		GetWorldTimerManager().SetTimer(Temp, Delegate, i * 0.5, false);	
	}
}

void AEnemySpawnerBase::ProcessSpawn()
{
	//전부 소진
	if (NumOfStock <= 0)
	{
		return;
	}
	
	//원뿔 범위로 변환
	const FTransform SpawnTransform = GetSpawnTransform();

	//트랜스폼만 전달하고 구체적인 소환 내용은 자식 클래스에 맡긴다
	SpawnSpecific(SpawnTransform);

	//재고 감소
	NumOfStock -= 1;
}

void AEnemySpawnerBase::SpawnSpecific(const FTransform& SpawnTransform)
{
	PRINTLOG(TEXT("FatalError! You Should Fully Override this Method in Subclass!"))
}
