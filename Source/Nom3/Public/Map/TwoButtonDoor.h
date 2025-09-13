 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MovingObject.h"
#include "TwoButtonDoor.generated.h"

class AInteractiveButton;

UCLASS()
class NOM3_API ATwoButtonDoor : public AMovingObject
{
	GENERATED_BODY()

public:
	ATwoButtonDoor();

	virtual void Tick(float DeltaTime) override;

	// 활성화 비활성화
	void ActivateButton(AInteractiveButton* Button);
	void DeactivateButton(AInteractiveButton* Button);

protected:
	virtual void BeginPlay() override;

	// 버튼 2개
	UPROPERTY(EditAnywhere, Category = "Button Door")
	AInteractiveButton* Button1;

	UPROPERTY(EditAnywhere, Category = "Button Door")
	AInteractiveButton* Button2;

private:
	// 문이 열려야 하는지 여부
	bool bShouldBeOpen;

	// 각 버튼의 활성화 상태
	bool bIsButton1Active;
	bool bIsButton2Active;

	// 문의 시작 위치와 목표 위치
	FVector StartLocation;
	FVector GlobalTargetLocation;

	// 문의 최대 속도
	float MaxSpeed;
	float Acceleration;

	// 오픈 결정
	void CheckDoorState();
};