// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankBase.h"
#include "ScoutShank.generated.h"

//전방 선언
class UScoutShankShooterComponent;
class UScoutShankDamageComponent;

UCLASS()
class NOM3_API AScoutShank : public AShankBase
{
	GENERATED_BODY()

public:
	AScoutShank();

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

protected:
	virtual void BeginPlay() override;

	//사격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UScoutShankShooterComponent> ShooterComp;

	//일반 데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UScoutShankDamageComponent> NormalDamageComp;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnShotDown(const FVector ShotDir) override;
};