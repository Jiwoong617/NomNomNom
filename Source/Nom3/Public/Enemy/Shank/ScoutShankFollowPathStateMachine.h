// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Components/ActorComponent.h"
#include "ScoutShankFollowPathStateMachine.generated.h"

//델리게이트 선언
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnDecideEvadeDirection, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UScoutShankFollowPathStateMachine : public UShankStateMachineBase
{
	GENERATED_BODY()

public:
	UScoutShankFollowPathStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};
