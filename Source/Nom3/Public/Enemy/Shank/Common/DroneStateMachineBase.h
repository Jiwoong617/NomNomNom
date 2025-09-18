// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/StateMachineBase.h"
#include "DroneStateMachineBase.generated.h"

//전방 선언
class ADroneBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDroneStateMachineBase : public UStateMachineBase
{
	GENERATED_BODY()

public:
	UDroneStateMachineBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADroneBase> OwnerDrone;
};
