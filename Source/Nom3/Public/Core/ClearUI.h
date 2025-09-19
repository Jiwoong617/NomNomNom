// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearUI.generated.h"

/**
 * 
 */
class AServitor;
class UImage;

UCLASS()
class NOM3_API UClearUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget)) UImage* header;
	UPROPERTY(meta=(BindWidget)) UImage* reward;
	
};
