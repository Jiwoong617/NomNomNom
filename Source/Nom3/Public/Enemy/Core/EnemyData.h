// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

UCLASS()
class NOM3_API UEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText EnemyName;

	UPROPERTY(EditAnywhere)
	int32 EnemyMaxHP;
};
