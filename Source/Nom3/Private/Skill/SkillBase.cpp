// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillBase.h"

void USkillBase::SetOwner(ANomPlayer* NewOwner)
{
	Owner = NewOwner;
}

const float USkillBase::GetCoolTime() const
{
	return CoolTime;
}

void USkillBase::UseSkill()
{
}
