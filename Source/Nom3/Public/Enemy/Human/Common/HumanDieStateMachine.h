// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanStateMachineBase.h"
#include "HumanDieStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UHumanDieStateMachine : public UHumanStateMachineBase
{
	GENERATED_BODY()

public:
	UHumanDieStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};