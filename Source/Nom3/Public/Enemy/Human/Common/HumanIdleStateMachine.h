// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanStateMachineBase.h"
#include "HumanIdleStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UHumanIdleStateMachine : public UHumanStateMachineBase
{
	GENERATED_BODY()

public:
	UHumanIdleStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};
