// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be interacted with by the player.
 */
class NOM3_API IInteractable
{
	GENERATED_BODY()

public:
	// Called when the player interacts with this object.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);
};