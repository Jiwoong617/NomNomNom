// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Dreg/DregShooterComponent.h"
#include "Core/ProjectilePoolWorldSubSystem.h"
#include "Enemy/Core/ProjectileBase.h"
#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Human/Dreg/NormalDreg.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UProjectilePoolWorldSubSystem;

UDregShooterComponent::UDregShooterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//정찰 생크 총알 블루프린트 클래스
	if (static ConstructorHelpers::FClassFinder<AProjectileBase>
		Finder(TEXT("/Game/Enemies/Shank/BP_ScoutShankBullet.BP_ScoutShankBullet_C"));
		Finder.Succeeded())
	{
		DregBulletClass = Finder.Class;
	}

	//사운드 큐 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/Dreg/Sound/SC_DregGunFire.SC_DregGunFire"));
		Finder.Succeeded())
	{
		DregFireSound = Finder.Object;
	}

	//자동 사격
	AutoFireRate = 4;
	AutoFireMin = 2;
	AutoFireMax = 4;
}

void UDregShooterComponent::BeginPlay()
{
	Super::BeginPlay();

	//소유자 드렉 획득
	OwnerDreg = Cast<ANormalDreg>(GetOwner());

	//플레이어 캐릭터 획득
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

void UDregShooterComponent::FireBulletOnce() const
{
	//소유자가 유효하지 않다면
	if (OwnerDreg->GetCurrentStateMachine() == nullptr)
	{
		return;
	}
	
	//회피 중이라면
	if (OwnerDreg->GetCurrentStateMachine()->GetClass() == OwnerDreg->EvadeStateMachine.GetClass())
	{
		return;
	}
	
	//목표 위치 획득
	const FVector TargetLocation = PlayerPawn->GetActorLocation();

	//목표 방향 연산
	FVector TargetDirection = (TargetLocation - GetComponentLocation()).GetSafeNormal();

	//목표 방향 랜덤
	TargetDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetDirection, ShootRandConeAngle);
	
	//목표 방향 로테이터
	const FRotator FireRotator = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), GetComponentLocation() + TargetDirection);

	//발사 사운드 재생
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DregFireSound, GetComponentLocation());

	//발사체 풀링 월드 서브시스템 획득
	if (const auto ProjectilePoolWorldSubSystem = GetWorld()->GetSubsystem<UProjectilePoolWorldSubSystem>())
	{
		//서브시스템에 발사체 풀링 요청
		ProjectilePoolWorldSubSystem->PopProjectile(DregBulletClass, GetComponentLocation(), FireRotator);
	}
}