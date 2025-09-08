// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovingObject.h"
#include "MovingPlatform.generated.h"
UCLASS()
class NOM3_API AMovingPlatform : public AMovingObject
{
	GENERATED_BODY()

public:
	AMovingPlatform();


protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

		
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float WaitTime;
	
	FTimerHandle MovementTimerHandle;

	//이동 및 반향
	void ToggleMovement();
    
	//원래 속도
	float OriginalSpeed;

private:

	
};