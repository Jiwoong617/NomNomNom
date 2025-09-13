// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShankShooterComponent.h"

#include "Core/ProjectilePoolWorldSubSystem.h"
#include "Enemy/Shank/ScoutShankProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UScoutShankShooterComponent::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 캐릭터 획득
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

UScoutShankShooterComponent::UScoutShankShooterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//일반 총탄 블루프린트 클래스
	if (static ConstructorHelpers::FClassFinder<AScoutShankProjectile>
		Finder(TEXT("/Game/Enemies/Shank/BP_ScoutShankBullet.BP_ScoutShankBullet_C"));
		Finder.Succeeded())
	{
		ScoutShankBulletClass = Finder.Class;
	}
}

void UScoutShankShooterComponent::FireBulletOnce() const
{
	//목표 위치 획득
	const FVector TargetLocation = PlayerPawn->GetActorLocation();

	//목표 방향 연산
	FVector TargetDirection = (TargetLocation - GetComponentLocation()).GetSafeNormal();

	//목표 방향 랜덤
	TargetDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetDirection, ShootRandConeAngle);
	
	//랜덤화된 목표 위치
	const FVector RandomizedSightLocation = TargetLocation + TargetDirection;
	
	//목표 방향 로테이터
	const FRotator FireRotator = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), RandomizedSightLocation);

	// //발사체 풀링 월드 서브시스템 획득
	// if (const auto ProjectilePoolWorldSubSystem = GetWorld()->GetSubsystem<UProjectilePoolWorldSubSystem>())
	// {
	// 	//서브시스템에 발사체 풀링 요청
	// 	ProjectilePoolWorldSubSystem->PopProjectile(ScoutShankBulletClass, GetComponentLocation(), FireRotator);
	// }
}