// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/EnemyTesterDamageComponent.h"

#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

UEnemyTesterDamageComponent::UEnemyTesterDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//총알 명중 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/ScoutShank/Sound/SC_ScoutShankBulletHitCue.SC_ScoutShankBulletHitCue"));
		Finder.Succeeded())
	{
		HitSound = Finder.Object;
	}
}