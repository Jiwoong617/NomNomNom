// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ThrowSkill.generated.h"

/**
 * 
 */
UCLASS()
class NOM3_API UThrowSkill : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void UseSkill() override;
};
