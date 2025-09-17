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
	Owner->LaunchCharacter( Owner->GetActorForwardVector() * (1500.f + Owner->GetVelocity().X)
	+ Owner->GetActorUpVector() * 300, true, true);
}
