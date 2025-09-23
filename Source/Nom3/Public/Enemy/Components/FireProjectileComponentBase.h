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

	//자동 사격 타이머 델리게이트
	FTimerDelegate AutoFireTimerDelegate;

	//자동 사격 간격
	float AutoFireRate;

	//자동 사격 1회 당 총알 범위
	int32 AutoFireMin;
	int32 AutoFireMax;

	//사격 카운트
	UPROPERTY(VisibleAnywhere)
	int32 FireCount;

	//사격 리미트
	UPROPERTY(VisibleAnywhere)
	int32 FireLimit;

	//사격 간격
	UPROPERTY(VisibleAnywhere)
	float FireInterval;

	//랜덤화
	UPROPERTY(EditAnywhere)
	float ShootRandConeAngle;

	//이전 사격으로부터 지난 시간
	UPROPERTY(VisibleAnywhere)
	float ElapsedTimeAfterLastFire;
	
public:
	UFireProjectileComponentBase();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//자동 사격 활성화
	UFUNCTION(BlueprintCallable)
	virtual void ActiveAutoFire();

	//자동 사격 비활성화
	UFUNCTION(BlueprintCallable)
	virtual void InactiveAutoFire();

	//자동 사격
	UFUNCTION(BlueprintCallable)
	void FireBulletMultiple();

	//한 발의 총탄 발사
	UFUNCTION(BlueprintCallable)
	virtual void FireBulletOnce() const;
};