// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/DamageWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//애니메이션 바인드
	if (ShowAndHideAnim)
	{
		// //시작 이벤트
		// FWidgetAnimationDynamicEvent StartEvent;
		// StartEvent.BindUFunction(this, TEXT("OnAnimationStarted"));
		// ShowAndHideAnim->BindToAnimationStarted(this, StartEvent);
		//
		// //종료 이벤트
		// FWidgetAnimationDynamicEvent FinishEvent;
		// FinishEvent.BindUFunction(this, TEXT("OnAnimationFinished"));
		// ShowAndHideAnim->BindToAnimationFinished(this, FinishEvent);
	}
	
	//슬롯 획득
	if (const auto Temp = Cast<UCanvasPanelSlot>(CanvasPanel))
	{
		//앵커 중앙
		FAnchors Anchor;
		Anchor.Minimum = FVector2D(0.5, 0.5);
		Anchor.Maximum = FVector2D(0.5, 0.5);
		Temp->SetAnchors(Anchor);

		//배치 중앙
		Temp->SetAlignment(FVector2D(0.5, 0.5));
	}

	//비가시화
	SetVisibility(ESlateVisibility::Hidden);
}

void UDamageWidget::ShowAndHide()
{
	//가시화
	SetVisibility(ESlateVisibility::Visible);

	//위젯 애니메이션 재생
	PlayAnimation(ShowAndHideAnim, 0, 0);
}

void UDamageWidget::OnAnimationStarted()
{
	
}

void UDamageWidget::OnAnimationFinished()
{
	
}
