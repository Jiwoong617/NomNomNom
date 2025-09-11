// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnWidgetAnimationStartedSignature)
DECLARE_MULTICAST_DELEGATE(FOnWidgetAnimationFinishedSignature)

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

	UFUNCTION()
	void DisplayDamage(int32 Damage);

	//애니메이션 시작 델리게이트
	FOnWidgetAnimationStartedSignature OnAnimationStartedDelegate;

	//애니메이션 종료 델리게이트
	FOnWidgetAnimationFinishedSignature OnAnimationFinishedDelegate;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> DamageText;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> ShowAndHideAnim;

	UFUNCTION()
	void OnShow();

	UFUNCTION()
	void OnHide();
};