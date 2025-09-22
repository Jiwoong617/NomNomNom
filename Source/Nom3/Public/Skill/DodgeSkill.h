// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "DodgeSkill.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class NOM3_API UDodgeSkill : public USkillBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere) UNiagaraSystem* NiagaraEffect;
	
public:
	UDodgeSkill();
	virtual void UseSkill() override;
};
