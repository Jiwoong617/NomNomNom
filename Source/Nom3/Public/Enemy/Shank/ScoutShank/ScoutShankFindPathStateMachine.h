// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/DroneStateMachineBase.h"
#include "ScoutShankFindPathStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UScoutShankFindPathStateMachine : public UDroneStateMachineBase
{
	GENERATED_BODY()

public:
	UScoutShankFindPathStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

protected:
	UFUNCTION()
	void DecideTargetLocation() const;

	UFUNCTION()
	void CalculatePaths() const;
};