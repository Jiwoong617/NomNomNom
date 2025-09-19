// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneStateMachineBase.h"
#include "DroneReverseThrustStateMachine.generated.h"

//전방 선언
class ADroneBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDroneReverseThrustStateMachine : public UDroneStateMachineBase
{
	GENERATED_BODY()

public:
	UDroneReverseThrustStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

protected:
	UPROPERTY(VisibleAnywhere)
	float InitVelocity;
};