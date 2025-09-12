// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Components/FireProjectileComponentBase.h"
#include "ScoutShankShooter.generated.h"

class ANomPlayer;
class AScoutShankBullet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UScoutShankShooter : public UFireProjectileComponentBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UScoutShankShooter();

	//목표로 하는 플레이어 캐릭터
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APawn> PlayerPawn;

	//정찰 생크가 발사하는 총탄 클래스
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AScoutShankBullet> ScoutShankBulletClass;
	
	//오버라이드
	virtual void FireBulletOnce() const override;
};
