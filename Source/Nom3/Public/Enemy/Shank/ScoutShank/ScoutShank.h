// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/DroneBase.h"
#include "ScoutShank.generated.h"

//전방 선언
class UScoutShankShooterComponent;
class UDroneDamageComponent;

UCLASS()
class NOM3_API AScoutShank : public ADroneBase
{
	GENERATED_BODY()

public:
	AScoutShank();

	virtual void BeginPlay() override;

	virtual void OnShotDown(const FVector ShotDir) override;
	
	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

protected:
	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneDamageComponent> DamageComp;

	//사격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UScoutShankShooterComponent> ShooterComp;

	//다이나믹 머터리얼 인스턴스
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DamageDynamicInstance;

	//정찰 드론만의 회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;
};