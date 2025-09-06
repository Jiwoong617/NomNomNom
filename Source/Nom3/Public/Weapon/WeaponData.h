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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EquipDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAimable;
};
