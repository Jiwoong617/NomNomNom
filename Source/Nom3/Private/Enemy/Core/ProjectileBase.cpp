// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/ProjectileBase.h"
#include "Components/SphereComponent.h"
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

	//생명 주기 설정
	AActor::SetLifeSpan(10);
}

void AProjectileBase::Active(const FVector& Location, const FRotator& Rotation)
{
	//활성화
	SetActorLocationAndRotation(Location, Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileFireInfo.FireLocation = Location;
}

void AProjectileBase::Inactivate()
{
	//비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}