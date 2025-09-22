// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanStateMachineBase.h"
#include "HumanEvadeStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UHumanEvadeStateMachine : public UHumanStateMachineBase
{
	GENERATED_BODY()

public:
	UHumanEvadeStateMachine();

	virtual void EnterState() override;

	virtual void ExecuteState() override;

	virtual void ExitState() override;

protected:
	UPROPERTY(VisibleAnywhere)
	FVector EvadeLocation;

	UPROPERTY(VisibleAnywhere)
	float InitWalkSpeed;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> EvadeAnimMontage;

	UFUNCTION()
	bool FindEvadeLocation(FVector& OutLocation);
};