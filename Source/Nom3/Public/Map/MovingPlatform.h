// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Interfaces/Interactable.h" 
#include "Map/MovingObject.h"
#include "MovingPlatform.generated.h"


UCLASS()
class NOM3_API AMovingPlaform : public AMovingObject
{
	GENERATED_BODY()

protected:
	// 게임 시작 시 호출됩니다.
	virtual void BeginPlay() override;

public:
	// 매 프레임 호출됩니다.
	virtual void Tick(float DeltaTime) override;

	FTimerHandle MovementHandle;
	float Movementdelay = 2.f;
};