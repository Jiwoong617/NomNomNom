// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/DodgeSkill.h"

#include "Core/NomPlayer.h"

UDodgeSkill::UDodgeSkill()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/Skill/Dash_Montage.Dash_Montage'"));
	if (anim.Succeeded())
		SkillMontage = anim.Object;
		
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/FX/NS_Jansang.NS_Jansang'"));
	if (niagara.Succeeded())
		NiagaraEffect = niagara.Object;
	
	Damage = 0;
	CoolTime = 15.f;
}

void UDodgeSkill::UseSkill()
{
	Owner->SetEffect(NiagaraEffect);
	
	Owner->LaunchCharacter( Owner->GetActorForwardVector() * (1500.f + Owner->GetVelocity().X)
	+ Owner->GetActorUpVector() * 300, true, true);

	if (SkillMontage)
		Owner->PlayTPSAnim(SkillMontage);
}
