// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UltimateSkill.h"

#include "Core/NomPlayer.h"

UUltimateSkill::UUltimateSkill()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/Skill/ulrta_Anim_Montage.ulrta_Anim_Montage'"));
	if (anim.Succeeded())
		SkillMontage = anim.Object;
	
	Damage = 3000;
	CoolTime = 90.f;
}

void UUltimateSkill::UseSkill()
{
	if (SkillMontage)
		Owner->PlayTPSAnim(SkillMontage);
}
