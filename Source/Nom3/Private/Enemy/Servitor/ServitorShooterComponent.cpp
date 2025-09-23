// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Servitor/ServitorShooterComponent.h"

#include "Core/ProjectilePoolWorldSubSystem.h"
#include "Enemy/Core/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UProjectilePoolWorldSubSystem;

UServitorShooterComponent::UServitorShooterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//정찰 생크 총알 블루프린트 클래스
	if (static ConstructorHelpers::FClassFinder<AProjectileBase>
		Finder(TEXT("/Game/Enemies/Drone/Servitor/BP_ServitorBullet.BP_ServitorBullet_C"));
		Finder.Succeeded())
	{
		ServitorBulletClass = Finder.Class;
	}

	//사운드 큐 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/Boss_Servitor/Sound/SC_ServitorFireCue.SC_ServitorFireCue"));
		Finder.Succeeded())
	{
		ServitorFireSound = Finder.Object;
	}

	//자동 사격
	AutoFireRate = 2;
	AutoFireMin = 1;
	AutoFireMax = 1;
}

void UServitorShooterComponent::FireBulletOnce() const
{
	//목표 위치 획득
	const FVector TargetLocation = PlayerPawn->GetActorLocation();

	//목표 방향 연산
	FVector TargetDirection = (TargetLocation - GetComponentLocation()).GetSafeNormal();

	//목표 방향 랜덤
	TargetDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetDirection, ShootRandConeAngle);
	
	//목표 방향 로테이터
	const FRotator FireRotator = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), GetComponentLocation() + TargetDirection);

	//발사 사운드 재생
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ServitorFireSound, GetComponentLocation());

	//발사체 풀링 월드 서브시스템 획득
	if (const auto ProjectilePoolWorldSubSystem = GetWorld()->GetSubsystem<UProjectilePoolWorldSubSystem>())
	{
		//서브시스템에 발사체 풀링 요청
		ProjectilePoolWorldSubSystem->PopProjectile(ServitorBulletClass, GetComponentLocation(), FireRotator);
	}
}

void UServitorShooterComponent::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 캐릭터 획득
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}