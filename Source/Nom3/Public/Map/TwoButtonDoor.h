 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwoButtonDoor.generated.h"

class AInteractiveButton;

UCLASS()
class NOM3_API ATwoButtonDoor : public AActor
{
	GENERATED_BODY()

public:
	ATwoButtonDoor();

	void ActivateButton(AInteractiveButton* Button);
	void DeactivateButton(AInteractiveButton* Button);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Button Door")
	AInteractiveButton* Button1;

	UPROPERTY(EditAnywhere, Category = "Button Door")
	AInteractiveButton* Button2;

	bool bIsButton1Active;
	bool bIsButton2Active;

	void CheckDoorState();
};