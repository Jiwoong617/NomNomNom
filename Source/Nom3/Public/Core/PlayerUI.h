// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UTextBlock;

UCLASS()
class NOM3_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TEXT_CurrentAmmo;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TEXT_MaxAmmo;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateAmmoUI(int32 currentammo, int32 maxammo);
};
