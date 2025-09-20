// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "Dreg.generated.h"

class UDregShooterComponent;

UCLASS()
class NOM3_API ADreg : public AHumanBase
{
	GENERATED_BODY()

public:
	ADreg();

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

protected:
	virtual void BeginPlay() override;

	//사격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDregShooterComponent> ShooterComp;

	//드렉 드론만의 회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;
};
