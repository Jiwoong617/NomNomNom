// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/DroneBase.h"
#include "SelfDestructShank.generated.h"

UCLASS()
class NOM3_API ASelfDestructShank : public ADroneBase
{
	GENERATED_BODY()

public:
	ASelfDestructShank();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnShotDown(const FVector ShotDir) override;

protected:
	//크리티컬 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneDamageComponent> CriticalDamageComp;

	//자폭 생크만의 특별한 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneStateMachineBase> FinalAssaultStateMachine;

	//다이나믹 머터리얼 인스턴스
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DamageDynamicInstance;
};
