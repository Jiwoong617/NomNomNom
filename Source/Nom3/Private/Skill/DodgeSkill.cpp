// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/DodgeSkill.h"

#include "Core/NomPlayer.h"

UDodgeSkill::UDodgeSkill()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/Skill/Dash_Montage.Dash_Montage'"));
	if (anim.Succeeded())
		SkillMontage = anim.Object;
	
	Damage = 0;
	CoolTime = 15.f;
}

void UDodgeSkill::UseSkill()
{
	Owner->LaunchCharacter( Owner->GetActorForwardVector() * (1500.f + Owner->GetVelocity().X)
	+ Owner->GetActorUpVector() * 300, true, true);

	if (SkillMontage)
		Owner->PlayTPSAnim(SkillMontage);
}
