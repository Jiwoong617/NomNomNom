// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/DamageWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//애니메이션 바인드
	if (ShowAndHideAnim)
	{
		//시작 이벤트
		FWidgetAnimationDynamicEvent StartEvent;
		StartEvent.BindDynamic(this, &UDamageWidget::OnShow);
		BindToAnimationStarted(ShowAndHideAnim, StartEvent);
		
		//종료 이벤트
		FWidgetAnimationDynamicEvent FinishEvent;
		FinishEvent.BindDynamic(this, &UDamageWidget::OnHide);
		BindToAnimationFinished(ShowAndHideAnim, FinishEvent);
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

void UDamageWidget::DisplayDamage(const int32 Damage)
{
	//가시화
	SetVisibility(ESlateVisibility::Visible);

	//데미지 텍스트 설정
	DamageText.Get()->SetText(FText::FromString(*FString::FromInt(Damage)));

	//위젯 애니메이션 재생
	PlayAnimation(ShowAndHideAnim);
}

void UDamageWidget::OnShow()
{
	OnAnimationStartedDelegate.Broadcast();
}

void UDamageWidget::OnHide()
{
	OnAnimationFinishedDelegate.Broadcast();
}