// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemySpawnerBase.h"

#include "Components/ArrowComponent.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "Enemy/Core/PlayerDetectVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nom3/Nom3.h"

AEnemySpawnerBase::AEnemySpawnerBase() :
	SpawnFrustumAngle(15),
	SpawnFrustumMin(50),
	SpawnFrustumMax(100),
	NumOfStock(8),
	SpawnCounter(0),
	SpawnLimiter(0),
	SpawnInterval(0.5),
	SpawnMin(4),
	SpawnMax(4),
	AutoSpawnRate(5)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스폰된 생크들이 사출되는 방향을 가리키는 애로우 컴포넌트
	SpawnFrustumDirection = CreateDefaultSubobject<UArrowComponent>(FName("SpawnFrustumDirection"));
	SpawnFrustumDirection->ArrowSize = 5;
	SpawnFrustumDirection->SetupAttachment(GetRootComponent());
}

void AEnemySpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	//스폰 인덱스
	StockIndex = 0;

	//스폰 차단기
	bBlockingSpawn = false;

	//비어 있는 만큼 채워 넣기
	for (int32 i = StockList.Num(); i < NumOfStock; i++)
	{
		TSubclassOf<AEnemyActorBase> Selected = nullptr;

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

void AEnemySpawnerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//발사 카운트가 작은 동안
	if (SpawnCounter < SpawnLimiter)
	{
		//발사 간격을 넘겼으므로
		if (ElapsedTimeAfterLastSpawn > SpawnInterval)
		{
			//1회 사격 호출
			ProcessSpawn();
		
			//발사 카운트 증가
			SpawnCounter++;

			//ElapsedTime 갱신
			ElapsedTimeAfterLastSpawn -= SpawnInterval;
		}

		//어쨌든 시간은 지나간다
		ElapsedTimeAfterLastSpawn += DeltaSeconds;
	}
	else
	{
		//스폰 차단기가 다시 풀린다
		bBlockingSpawn = false;
	}
}

void AEnemySpawnerBase::OnNoticePawn(AActor* DetectedPawn)
{
	
}

void AEnemySpawnerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BindPlayerDetectVolume == nullptr)
	{
		BindPlayerDetectVolume = GetWorld()->SpawnActor<APlayerDetectVolume>();
		BindPlayerDetectVolume->SetActorLocation(GetActorLocation());
		BindPlayerDetectVolume->BindSpawnerAndEnemy.Add(this);
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

void AEnemySpawnerBase::ActiveAutoSpawn()
{
	//타이머 델리게이트 바인드
	AutoSpawnTimerDelegate.BindLambda([this]()
	{
		//자동 스폰 메서드 호출
		RequestSpawnEnemies();
	});

	//최초 실행
	AutoSpawnTimerDelegate.Execute();

	//자동 사격 호출
	GetWorld()->GetTimerManager().SetTimer(AutoSpawnTimerHandle, AutoSpawnTimerDelegate, AutoSpawnRate, true);
}

void AEnemySpawnerBase::InactivateAutoSpawn()
{
	//사격 타이머 비활성화
	GetWorld()->GetTimerManager().ClearTimer(AutoSpawnTimerHandle);
}

void AEnemySpawnerBase::RequestSpawnEnemies()
{
	//이전 요청이 아직 마무리 되지 않았다면
	if (bBlockingSpawn)
	{
		return;
	}

	//블로킹 활성화
	bBlockingSpawn = true;

	//스폰 지나간 시간 초기화
	ElapsedTimeAfterLastSpawn = 0;
	
	//스폰 카운트 초기화
	SpawnCounter = 0;

	//스폰 리미터 초기화
	SpawnLimiter = FMath::RandRange(SpawnMin, SpawnMax);
}

void AEnemySpawnerBase::ProcessSpawn()
{
	//전부 소진
	if (NumOfStock <= 0)
	{
		InactivateAutoSpawn();
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