// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Core/EnemySpawnerBase.h"

<<<<<<< Updated upstream
=======
#include "Components/ArrowComponent.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "Enemy/Core/PlayerDetectVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nom3/Nom3.h"
>>>>>>> Stashed changes

// Sets default values
AEnemySpawnerBase::AEnemySpawnerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
<<<<<<< Updated upstream
	PrimaryActorTick.bCanEverTick = true;
=======
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
>>>>>>> Stashed changes
}

void AEnemySpawnerBase::OnDetectPlayerPawn()
{
	
}