// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UltimateSkill.h"

#include "Core/NomPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	{
		FVector LeftDir = -Owner->GetActorRightVector();
		FVector NewLocation = Owner->GetTpsComp()->GetComponentLocation() + LeftDir * 150.f;
		Owner->GetTpsComp()->SetWorldLocation(NewLocation);
		//Owner->GetTpsComp()->AddWorldRotation(FRotator(0, 0, -20));
		Owner->GetTpsCamArm()->TargetArmLength = 450;
		
		Owner->GetCharacterMovement()->GravityScale = 0.7f;
		Owner->LaunchCharacter(Owner->GetActorUpVector() * 1000, true, true);
		
		Owner->PlayTPSAnim(SkillMontage);
	}
}

void UUltimateSkillNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
}
