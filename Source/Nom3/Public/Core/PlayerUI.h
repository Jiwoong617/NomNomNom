// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

UCLASS()
class NOM3_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget)) UProgressBar* HpBar;
	
	UPROPERTY(meta=(BindWidget)) UProgressBar* Bar_Skill1;
	UPROPERTY(meta=(BindWidget)) UProgressBar* Bar_Skill2;
	UPROPERTY(meta=(BindWidget)) UProgressBar* Bar_Skill3;

	UPROPERTY(meta=(BindWidget)) UImage* IMG_Weapon1;
	UPROPERTY(meta=(BindWidget)) UImage* IMG_Weapon2;
	UPROPERTY(meta=(BindWidget)) UImage* IMG_Weapon3;

	UPROPERTY(meta=(BindWidget)) UTextBlock* TEXT_CurrentAmmo;
	UPROPERTY(meta=(BindWidget)) UTextBlock* TEXT_MaxAmmo;
	UPROPERTY(meta=(BindWidget)) UTextBlock* TEXT_Weapon2Ammo;
	UPROPERTY(meta=(BindWidget)) UTextBlock* TEXT_Weapon3Ammo;

	UPROPERTY() TArray<UImage*> WeaponImgs;
	UPROPERTY() TArray<UTextBlock*> AmmoTexts;
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAmmoUI(int32 currentammo, int32 maxammo);

	UFUNCTION(BlueprintCallable)
	void UpdateHealthUI(float health, float maxhealth);

	UFUNCTION(BlueprintCallable)
	void UpdateEquipedWeaponUI(int32 idx, UTexture2D* img, int32 ammo, int32 maxammo = 0);
};
