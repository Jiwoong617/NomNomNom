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


UCLASS()
class NOM3_API UUltimateSkillNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};