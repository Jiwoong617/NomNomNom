// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatus.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class NOM3_API UEnemyStatus : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget=NameText))
	TObjectPtr<UTextBlock> NameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget=HPBar))
	TObjectPtr<UProgressBar> HPBar;

	UFUNCTION()
	void SetName(const FText& Name) const;

	UFUNCTION()
	void UpdateHPBar(const float HPPer) const;
};
