// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneStateMachineBase.h"
#include "Components/ActorComponent.h"
#include "DroneFollowPathStateMachine.generated.h"

//델리게이트 선언
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnDecideEvadeDirection, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDroneFollowPathStateMachine : public UDroneStateMachineBase
{
	GENERATED_BODY()

public:
	UDroneFollowPathStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};
