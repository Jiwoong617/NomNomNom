// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DamageComponent.h"
#include "EnemyTesterDamageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UEnemyTesterDamageComponent : public UDamageComponent
{
	GENERATED_BODY()

public:
	UEnemyTesterDamageComponent();

protected:
	UPROPERTY(VisibleAnywhere, Category="Damage")
	TObjectPtr<USoundBase> HitSound;
};
