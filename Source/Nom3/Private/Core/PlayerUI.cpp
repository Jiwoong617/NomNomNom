// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerUI.h"

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

	//TODO : 버그 있음 고쳐야됨
	WeaponImgs[idx]->SetBrushFromTexture(img, false);
	AmmoTexts[idx]->SetText(FText::AsNumber(ammo));
}
