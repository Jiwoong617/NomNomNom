// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/DodgeSkill.h"

#include "Core/NomPlayer.h"

UDodgeSkill::UDodgeSkill()
{
	Damage = 0;
	CoolTime = 20.f;
}

void UDodgeSkill::UseSkill()
{
	Owner->LaunchCharacter(Owner->GetActorForwardVector() * 800.f
	+ Owner->GetActorUpVector() * 200, true, true);
}
