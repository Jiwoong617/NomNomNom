// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineBase.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract)
class NOM3_API UStateMachineBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateMachineBase();

	UFUNCTION()
	virtual void EnterState();

	UFUNCTION()
	virtual void ExecuteState();

	UFUNCTION()
	virtual void ExitState();

protected:
	UPROPERTY(VisibleAnywhere)
	bool bReadyToExecute;

	UPROPERTY(VisibleAnywhere)
	float ElapsedTimeInState;

	UPROPERTY(VisibleAnywhere)
	float LimitTimeInState;
};
