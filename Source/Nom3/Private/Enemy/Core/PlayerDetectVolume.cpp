// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/PlayerDetectVolume.h"
#include "Components/SphereComponent.h"
#include "Core/NomPlayer.h"
#include "Enemy/Core/EnemySpawnerBase.h"
#include "Interfaces/NoticePawn.h"

APlayerDetectVolume::APlayerDetectVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectSphereComp = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectSphereComp"));
	PlayerDetectSphereComp->SetCollisionProfileName(FName("PawnDetect"));
	PlayerDetectSphereComp->SetSphereRadius(500);
	SetRootComponent(PlayerDetectSphereComp);
}

void APlayerDetectVolume::BeginPlay()
{
	Super::BeginPlay();

	//비활성화 상태
	bActivate = false;
}

void APlayerDetectVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//이미 활성화됐다면
	if (bActivate)
	{
		return;
	}

	//활성화 마크
	bActivate = true;

	//충돌 비활성화
	PlayerDetectSphereComp->SetCollisionProfileName(FName("NoCollision"));

	//바인딩 되어 있는 폰 인식 액터들에게 전파
	for (const auto Bind : BindSpawnerAndEnemy)
	{
		if (const auto Notice = Cast<INoticePawn>(Bind))
		{
			Notice->OnNoticePawn(OtherActor);	
		}
	}
}