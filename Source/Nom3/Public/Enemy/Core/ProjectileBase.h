// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ProjectileBase.generated.h"

//전방 선언
class USphereComponent;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	FProjectileInfo() : Damage(0), Speed(0) {};
	
	FProjectileInfo(const float Damage, const float Speed) : Damage(Damage), Speed(Speed) {};

	//발사체의 기본 피해량
	UPROPERTY(BlueprintReadWrite)
	float Damage;

	//발사체의 기본 속력
	UPROPERTY(BlueprintReadWrite)
	float Speed;

	//발사체의 거리에 따른 데미지 경감 커브
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCurveFloat> DamageCurve;
};

UCLASS()
class NOM3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	//발사체 정보 구조체
	UPROPERTY(VisibleAnywhere)
	FProjectileInfo ProjectileInfo;
	
	//발사 정보 구조체
	UPROPERTY(VisibleAnywhere)
	FFireInfo ProjectileFireInfo;

	//활성화
	UFUNCTION()
	void Active(const FVector& Location, const FRotator& Rotation);

	//비활성화
	UFUNCTION()
	void Inactivate();

protected:

	//풀링 타이머 핸들
	FTimerHandle PoolingTimerHandle;

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	//발사체 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;
};