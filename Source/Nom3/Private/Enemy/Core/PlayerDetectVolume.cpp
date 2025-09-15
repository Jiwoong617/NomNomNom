// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/PlayerDetectVolume.h"
#include "Components/SphereComponent.h"
#include "Core/NomPlayer.h"
#include "Enemy/Shank/ShankSpawner.h"

APlayerDetectVolume::APlayerDetectVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectSphereComp = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectSphereComp"));
	PlayerDetectSphereComp->SetCollisionProfileName(FName("OverlapOnlyPawn"));
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

	//진입한 폰이 플레이어였다면
	if (auto Temp = Cast<ANomPlayer>(OtherActor))
	{
		//유효한 스포너가 연결되어 있다면
		if (BindSpawner)
		{
			BindSpawner->OnDetectPlayerPawn();
		}	
	}
}