// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageActor.generated.h"

class UDamageWidget;
class UWidgetComponent;

UCLASS()
class NOM3_API ADamageActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageActor();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageWidget> DamageWidget;

public:
	UFUNCTION()
	void DisplayDamage(int32 Damage) const;

	UFUNCTION()
	void Deactivate();
};