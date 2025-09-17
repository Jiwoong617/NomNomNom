// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DamageComponent.h"
#include "DregDamageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDregDamageComponent : public UDamageComponent
{
	GENERATED_BODY()

public:
	UDregDamageComponent();

	virtual void OnHitBody(FFireInfo& Info) override;

	virtual void OnHitHead(FFireInfo& Info) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Damage")
	TObjectPtr<USoundBase> HitSound;
};
