// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FireProjectileComponentBase.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UFireProjectileComponentBase : public USceneComponent
{
	GENERATED_BODY()

protected:
	//자동 사격 타이머 핸들러
	FTimerHandle AutoFireTimerHandle;

	//사격 간격 타이머 핸들러
	FTimerHandle FireIntervalTimerHandle;

	//자동 사격 간격
	float AutoFireRate;

	//자동 사격 1회 당 총알 범위
	int32 AutoFireMin;
	int32 AutoFireMax;

	//사격 카운트
	UPROPERTY(VisibleAnywhere)
	int32 FireCount;

	//랜덤화
	UPROPERTY(EditAnywhere)
	float ShootRandConeAngle;
	
public:
	UFireProjectileComponentBase();

	//자동 사격 활성화
	UFUNCTION(BlueprintCallable)
	virtual void ActiveAutoFire();

	//자동 사격 비활성화
	UFUNCTION(BlueprintCallable)
	virtual void InactiveAutoFire();

	//자동 사격
	UFUNCTION(BlueprintCallable)
	void FireBulletMultipleForInnerReq(const float Delay);
	
	//외부 요청으로 복수의 총탄 발사
	UFUNCTION(BlueprintCallable)
	void FireBulletMultipleForOuterReq(int Num, float Delay);

	//내외부 요청으로 복수의 총탄 발사
	void FireBulletMultiple(const int Num, float Delay);

	//내외부 요청으로 하나의 총탄 발사
	UFUNCTION(BlueprintCallable)
	virtual void FireBulletOnce() const;
};