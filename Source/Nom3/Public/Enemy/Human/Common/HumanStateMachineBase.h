// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/StateMachineBase.h"
#include "HumanStateMachineBase.generated.h"

//전방 선언
class AHumanBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UHumanStateMachineBase : public UStateMachineBase
{
	GENERATED_BODY()

public:
	UHumanStateMachineBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AHumanBase> OwnerHuman;
};
