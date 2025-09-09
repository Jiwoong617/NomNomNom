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

<<<<<<< HEAD
	FTimerHandle MovementHandle;
	float Movementdelay = 2.f;
=======
		
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float WaitTime;
	
	FTimerHandle MovementTimerHandle;

	//이동 및 반향
	void ToggleMovement();
    
	//원래 속도
	float OriginalSpeed;

private:

	
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
};