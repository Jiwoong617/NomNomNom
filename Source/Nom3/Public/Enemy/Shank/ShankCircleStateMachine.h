// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/Core/StateMachineBase.h"
#include "ShankCircleStateMachine.generated.h"

//델리게이트 선언
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnDecideEvadeDirection, float)

//전방 선언
class AShankBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UShankCircleStateMachine : public UStateMachineBase
{
	GENERATED_BODY()

public:
	UShankCircleStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

	UFUNCTION()
	void DecideTargetLocation() const;

	UFUNCTION()
	void ResetCirclingTimerHandle(float Timing);

protected:
	virtual void BeginPlay() override;

	FTimerDelegate DecideTargetLocationDelegate;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle CirclingTimerHandle;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AShankBase> OwnerShank;
};
