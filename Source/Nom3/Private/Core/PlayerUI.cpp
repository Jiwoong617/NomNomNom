// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Weapon/WeaponComponent.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerUI::UpdateAmmoUI(int32 currentammo, int32 maxammo)
{
	TEXT_CurrentAmmo->SetText(FText::AsNumber(currentammo));
	TEXT_MaxAmmo->SetText(FText::AsNumber(maxammo));
}

void UPlayerUI::UpdateHealthUI(float health, float maxhealth)
{
	HpBar->SetPercent(health / maxhealth);
}
