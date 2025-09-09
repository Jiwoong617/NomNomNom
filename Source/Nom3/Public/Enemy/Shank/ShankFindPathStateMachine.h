// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "ShankFindPathStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UShankFindPathStateMachine : public UShankStateMachineBase
{
	GENERATED_BODY()

public:
	UShankFindPathStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DecideTargetLocation() const;
};