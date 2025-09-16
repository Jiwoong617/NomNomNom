// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Damage/DamageActorPoolGameInstanceSubsystem.h"

AEnemyBase::AEnemyBase() :
	Health(100)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::OnAimByPlayerSight()
{
	
}

void AEnemyBase::OnDamaged(const FFireInfo& Info)
{
	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	//플레어어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//데미지 액터 풀링 서브시스템 획득
	if (auto Temp = GetWorld()->GetGameInstance()->GetSubsystem<UDamageActorPoolGameInstanceSubsystem>())
	{
		DamageActorPool = Temp;
	}
}