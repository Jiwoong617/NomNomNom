// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerUI.h"

#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Weapon/WeaponComponent.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponImgs.Add(IMG_Weapon1);
	WeaponImgs.Add(IMG_Weapon2);
	WeaponImgs.Add(IMG_Weapon3);

	AmmoTexts.Add(TEXT_CurrentAmmo);
	AmmoTexts.Add(TEXT_Weapon2Ammo);
	AmmoTexts.Add(TEXT_Weapon3Ammo);
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

void UPlayerUI::UpdateEquipedWeaponUI(int32 idx, UTexture2D* img, int32 ammo, int32 maxammo)
{
	if (maxammo)
		UpdateAmmoUI(ammo, maxammo);

	WeaponImgs[idx]->SetBrushFromTexture(img, false);
    AmmoTexts[idx]->SetText(FText::AsNumber(ammo));
}

void UPlayerUI::UpdateSkill1Cooldown(float cool, float coolTime)
{
    if (Bar_Skill1)
    {
        if (cool < coolTime)
            Bar_Skill1->SetPercent(FMath::Clamp(cool / coolTime, 0.f, 1.f));
        else
            Bar_Skill1->SetPercent(1.f);
    }
}

void UPlayerUI::UpdateSkill2Cooldown(float cool, float coolTime)
{
	if (Bar_Skill2)
	{
		if (cool < coolTime)
			Bar_Skill2->SetPercent(FMath::Clamp(cool / coolTime, 0.f, 1.f));
		else
			Bar_Skill2->SetPercent(1.f);
	}
}

void UPlayerUI::UpdateSkill3Cooldown(float cool, float coolTime)
{
	if (Bar_Skill3)
	{
		if (cool < coolTime)
			Bar_Skill3->SetPercent(FMath::Clamp(cool / coolTime, 0.f, 1.f));
		else
			Bar_Skill3->SetPercent(1.f);
	}
}

void UPlayerUI::IsPlayerDead(bool isDaed)
{
	if (isDaed)
		Panel_Dead->SetVisibility(ESlateVisibility::Visible);
	else
		Panel_Dead->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerUI::UpdateRespawnBar(float value)
{
	Bar_Respawn->SetPercent(value);
}

void UPlayerUI::OnClear()
{
	CB_Mission->SetIsChecked(true);
	PlayAnimation(ClearAnim);
}
