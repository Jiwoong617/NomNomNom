// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyStatus.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyStatus::SetName(const FText& Name) const
{
	NameText->SetText(Name);
}

void UEnemyStatus::UpdateHPBar(const float HPPer) const
{
	HPBar->SetPercent(HPPer);
}

void UEnemyStatus::Show() const
{
	NameText->SetVisibility(ESlateVisibility::Visible);
	HPBar->SetVisibility(ESlateVisibility::Visible);
}

void UEnemyStatus::Hide() const
{
	NameText->SetVisibility(ESlateVisibility::Hidden);
	HPBar->SetVisibility(ESlateVisibility::Hidden);
}