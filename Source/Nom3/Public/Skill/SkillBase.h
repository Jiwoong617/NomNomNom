// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillBase.generated.h"

class ANomPlayer;
class UAnimMontage;


UCLASS()
class NOM3_API USkillBase : public UObject
{
	GENERATED_BODY()

protected:
	int32 Damage;
	float CoolTime;

	UPROPERTY()
	ANomPlayer* Owner;
	UPROPERTY()
	UAnimMontage* SkillMontage;
	
public:
	void SetOwner(ANomPlayer* NewOwner);
	const float GetCoolTime() const;
	virtual void UseSkill();
};
