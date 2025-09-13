// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Core/ProjectilePoolWorldSubSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 부착
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("Projectile"), true);
	SetRootComponent(SphereComp);

	//발사체 이동 컴포넌트 부착
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
}

void AProjectileBase::Active(const FVector& Location, const FRotator& Rotation)
{
	//활성화
	SetActorLocationAndRotation(Location, Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	//사격 정보 업데이트
	FireInfo.FireLocation = Location;

	//이동 방향 전환
	ProjectileMoveComp->Velocity = FireInfo.ProjectileSpeed * GetActorForwardVector();

	//10초 후에 풀에 반환
	GetWorldTimerManager().SetTimer(PoolingTimerHandle, [this]()
	{
		//반환 호출
		Inactivate();
	}, 10, false);
}

void AProjectileBase::Inactivate()
{
	//풀링 타이머 핸들 비활성
	if (PoolingTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PoolingTimerHandle);
	}
	
	//비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//풀에 반환한다
	if (const auto Subsystem = GetWorld()->GetSubsystem<UProjectilePoolWorldSubSystem>())
	{
		Subsystem->PushProjectile(this);
	}
}