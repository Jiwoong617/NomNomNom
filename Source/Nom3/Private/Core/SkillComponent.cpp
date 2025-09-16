// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SkillComponent.h"
#include "Core/NomPlayer.h"
#include "Skill/DodgeSkill.h"
#include "Skill/SkillBase.h"
#include "Skill/UltimateSkill.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (ANomPlayer* OwnerPlayer = Cast<ANomPlayer>(GetOwner()))
    {
        Owner = OwnerPlayer;
        
        if (!DodgeSkill)
        {
            DodgeSkill = NewObject<UDodgeSkill>(this);
            DodgeSkill->SetOwner(Owner);
            DodgeCooldown = DodgeSkill->GetCoolTime();
        }
        if (!UltimateSkill)
        {
            UltimateSkill = NewObject<UUltimateSkill>(this);
            UltimateSkill->SetOwner(Owner);
            UltimateCooldown = UltimateSkill->GetCoolTime();
        }
    }
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (DodgeCooldown <= DodgeSkill->GetCoolTime())
    {
        DodgeCooldown = FMath::Min(DodgeCooldown + DeltaTime, DodgeSkill->GetCoolTime());
        DodgeSkillCoolDownDelegate.Broadcast(DodgeCooldown, DodgeSkill->GetCoolTime());
    }

    if (UltimateCooldown <= UltimateSkill->GetCoolTime())
    {
        UltimateCooldown = FMath::Min(UltimateCooldown + DeltaTime, UltimateSkill->GetCoolTime());
        UltimateSkillCoolDownDelegate.Broadcast(UltimateCooldown, UltimateSkill->GetCoolTime());
    }
}

bool USkillComponent::UseDodgeSkill()
{
    if (DodgeCooldown < DodgeSkill->GetCoolTime())
        return false;

    DodgeSkill->UseSkill();
    DodgeCooldown = 0.f;
    return true;
}

bool USkillComponent::UseUltimateSkill()
{
    if (UltimateCooldown < UltimateSkill->GetCoolTime())
        return false;
    
    UltimateSkill->UseSkill();
    UltimateCooldown = 0.f;
    return true;
}
