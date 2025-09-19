// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/DroneStateMachineBase.h"
#include "SelfDestructFinalAssaultStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API USelfDestructFinalAssaultStateMachine : public UDroneStateMachineBase
{
	GENERATED_BODY()

public:
	USelfDestructFinalAssaultStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};
