// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ProjectileBase.generated.h"

//전방 선언
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class NOM3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	virtual void BeginPlay() override;
	
	//발사 정보 구조체
	UPROPERTY(VisibleAnywhere)
	FFireInfo FireInfo;

	//활성화
	UFUNCTION()
	void Active(const FVector& Location, const FRotator& Rotation);

	//비활성화
	UFUNCTION()
	void Inactivate();

protected:

	//풀링 타이머 핸들
	FTimerHandle PoolingTimerHandle;

	//풀링 델리게이트
	FTimerDelegate PoolingDelegate;

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	//발사체 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;
};