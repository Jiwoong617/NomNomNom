// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/DroneBase.h"
#include "Servitor.generated.h"

class UServitorShooterComponent;

UCLASS()
class NOM3_API AServitor : public ADroneBase
{
	GENERATED_BODY()

public:
	AServitor();

	virtual void BeginPlay() override;

	virtual void OnShotDown(const FVector ShotDir) override;

	// clear Ui 관련 함수
	UFUNCTION()
	void HandleDeath();

protected:
	//일반적인 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> DamageComp;

	//크리티컬 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> CriticalDamageComp;

	//사격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UServitorShooterComponent> ShooterComp;
};
