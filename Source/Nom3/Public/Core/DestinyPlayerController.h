// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DestinyPlayerController.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class NOM3_API ADestinyPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	void ShowClearUI();

	UPROPERTY(EditAnywhere,Category="UI")
	TSubclassOf<UUserWidget> ClearUI;
};
