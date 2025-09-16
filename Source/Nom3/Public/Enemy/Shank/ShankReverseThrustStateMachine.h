// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "ShankReverseThrustStateMachine.generated.h"

//전방 선언
class AShankBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UShankReverseThrustStateMachine : public UShankStateMachineBase
{
	GENERATED_BODY()

public:
	UShankReverseThrustStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

protected:
	UPROPERTY(VisibleAnywhere)
	float InitVelocity;
};