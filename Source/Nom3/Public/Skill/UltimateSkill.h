// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "UltimateSkill.generated.h"

/**
 * 
 */
UCLASS()
class NOM3_API UUltimateSkill : public USkillBase
{
	GENERATED_BODY()
	
public:
	UUltimateSkill();
	virtual void UseSkill() override;
};
