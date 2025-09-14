// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DamageComponent.h"
#include "ScoutShankDamageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UScoutShankDamageComponent : public UDamageComponent
{
	GENERATED_BODY()

public:
	UScoutShankDamageComponent();

	virtual void OnHitBody(FFireInfo& Info) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Damage")
	TObjectPtr<USoundBase> HitSound;
};