// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/StateMachineBase.h"
#include "ShankStateMachineBase.generated.h"

//전방 선언
class AShankBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UShankStateMachineBase : public UStateMachineBase
{
	GENERATED_BODY()

public:
	UShankStateMachineBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AShankBase> OwnerShank;
};
