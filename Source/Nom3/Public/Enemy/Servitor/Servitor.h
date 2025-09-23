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

	virtual void OnDie() override;

	virtual void OnDamaged(FFireInfo Info) override;

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

	//스파크 나이아가라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> SparkNiagara;
};
