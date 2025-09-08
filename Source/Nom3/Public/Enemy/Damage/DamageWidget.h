// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//델리게이트 선언
DECLARE_DELEGATE_OneParam(FOnWidgetAnimationFinishedSignature, AActor)

//전방 선언
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

UCLASS()
class NOM3_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void ShowAndHide();

	FOnWidgetAnimationFinishedSignature OnAnimationFinishedDelegate;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> DamageText;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> ShowAndHideAnim;
	
	void OnAnimationStarted();
	
	void OnAnimationFinished();

};