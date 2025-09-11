// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Components/ActorComponent.h"
#include "ShankFollowPathStateMachine.generated.h"

//델리게이트 선언
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnDecideEvadeDirection, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UShankFollowPathStateMachine : public UShankStateMachineBase
{
	GENERATED_BODY()

public:
	UShankFollowPathStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;
};
