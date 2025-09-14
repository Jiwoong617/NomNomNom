// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	HandCannon = 0			UMETA(DisplayName = "HandCannon"),
	Rifle = 1				UMETA(DisplayName = "Rifle"),
	RocketLauncher = 2		UMETA(DisplayName = "RocketLauncher")
};

UCLASS(BlueprintType)
class NOM3_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	//장착 및 장전 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EquipDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	//탄창 및 발사 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	//조준
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAimable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ADS_FOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimDuration;

	//반동 pitch-상하, yaw-좌우
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilPitchMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilPitchMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilYawMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilYawMax;

	//비조준 시 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpread;

	//무기 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponImg;
};
