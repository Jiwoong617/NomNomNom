// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ThrowSkill.h"

#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "Nom3/Nom3.h"
#include "Weapon/Grenade.h"

UThrowSkill::UThrowSkill()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/Skill/grenade_Anim_Montage.grenade_Anim_Montage'"));
	if (anim.Succeeded())
		SkillMontage = anim.Object;
	
	Damage = 100;
	CoolTime = 20.f;
}

void UThrowSkill::UseSkill()
{
	GetWorld()->SpawnActor<AGrenade>(AGrenade::StaticClass()
		, Owner->GetFpsCam()->GetComponentLocation() + Owner->GetFpsCam()->GetRightVector() * -30
		, FRotator(0))
		->Init(Owner->GetFpsCam()->GetForwardVector(), 1000 + Owner->GetVelocity().X, Owner);

	if (SkillMontage)
		Owner->PlayFPSAnim(SkillMontage);
}
