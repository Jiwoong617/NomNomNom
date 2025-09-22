// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Components/FireProjectileComponentBase.h"
#include "DregShooterComponent.generated.h"

class ANormalDreg;
class AProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDregShooterComponent : public UFireProjectileComponentBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UDregShooterComponent();

	//작착된 드렉 캐릭터
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ANormalDreg> OwnerDreg;

	//목표로 하는 플레이어 캐릭터
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APawn> PlayerPawn;

	//드렉이 발사하는 총탄 클래스
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AProjectileBase> DregBulletClass;

	//드렉 발사 사운드
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> DregFireSound;

	//오버라이드
	virtual void FireBulletOnce() const override;
};