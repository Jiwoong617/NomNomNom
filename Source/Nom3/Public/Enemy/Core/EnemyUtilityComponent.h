// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyUtilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UEnemyUtilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyUtilityComponent();

	virtual void BeginPlay() override;
};
