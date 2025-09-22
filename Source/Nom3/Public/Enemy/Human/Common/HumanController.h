// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HumanController.generated.h"

class AHumanBase;

UCLASS()
class NOM3_API AHumanController : public AAIController
{
	GENERATED_BODY()

public:
	AHumanController();

	void OnMoveTaskCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AHumanBase> OwnerHuman;
};
