// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "DodgeSkill.generated.h"

/**
 * 
 */
UCLASS()
class NOM3_API UDodgeSkill : public USkillBase
{
	GENERATED_BODY()

public:
	UDodgeSkill();
	virtual void UseSkill() override;
};
