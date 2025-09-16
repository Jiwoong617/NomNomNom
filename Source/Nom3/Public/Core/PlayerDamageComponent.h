// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponent.h"
#include "PlayerDamageComponent.generated.h"


class ANomPlayer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UPlayerDamageComponent : public UDamageComponent
{
	GENERATED_BODY()

public:
	UPlayerDamageComponent();

protected:
	virtual void OnHitBody(FFireInfo& Info) override;

	virtual void OnHitHead(FFireInfo& Info) override;
};
