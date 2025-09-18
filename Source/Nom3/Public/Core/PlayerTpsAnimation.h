// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerTpsAnimation.generated.h"

class ANomPlayer;
/**
 * 
 */
UCLASS()
class NOM3_API UPlayerTpsAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ANomPlayer* Owner;

	UFUNCTION()
	void PlaySkillAnim(UAnimMontage* montage);
};
