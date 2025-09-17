// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemyCharacterBase.h"
#include "Dreg.generated.h"

class UDregDamageComponent;
class UDregStateMachineBase;

UCLASS()
class NOM3_API ADreg : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	ADreg();

	virtual void BeginPlay() override;

	//일반적인 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDregDamageComponent> DamageComp;

	//크리티컬 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDregDamageComponent> CriticalDamageComp;

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

	//일반적인 데미지 인터페이스 구현
	virtual void OnDamaged(FFireInfo Info) override;

	//크리티컬 데미지 인터페이스 구현
	virtual void OnCriticalDamaged(FFireInfo Info) override;

protected:

	//회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
