// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UltimateSkill.h"

#include "Core/NomPlayer.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Skill/ThrowingDagger.h"

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
	
	ANomPlayer* Player = Cast<ANomPlayer>(MeshComp->GetOwner());
	if (Player == nullptr) return;
	
	UCameraComponent* TpsCam = Player->GetTpsCam();
	FVector Loc = Player->GetActorLocation();
	FVector CamFwd = TpsCam->GetForwardVector();
	FVector SpawnLocation = MeshComp->GetSocketLocation(TEXT("WeaponSocket"));
	
	float MaxRange = 15000.f;
	float HalfFovDeg = TpsCam->FieldOfView * 0.5f;
	
	
	TArray<AEnemyActorBase*> Enemies;
	for (AEnemyActorBase* Enemy : TActorRange<AEnemyActorBase>(MeshComp->GetWorld()))
	{
		if (!IsValid(Enemy)) continue;
		if (!Enemy || Enemy->GetComponentByClass<UEnemyHealthComponent>()->GetHP() <= 0) continue;

		float Dist = (Enemy->GetActorLocation() - Loc).Size();
		if (Dist > MaxRange) continue;

		FVector Dir = (Enemy->GetActorLocation() - Loc) / Dist;
		float CosAngle = FVector::DotProduct(CamFwd, Dir);
		float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(CosAngle, -1.0f, 1.0f)));
		if (AngleDeg <= HalfFovDeg)
			Enemies.Add(Enemy);
	}
	
	if (Enemies.Num() == 0)
		return;
	Enemies.Sort([&](const AEnemyActorBase& A, const AEnemyActorBase& B)
	{
		return FVector::Dist(Player->GetActorLocation(), A.GetActorLocation()) <
			   FVector::Dist(Player->GetActorLocation(), B.GetActorLocation());
	});
	
	TArray<AEnemyActorBase*> Selected;
	for (int32 i = 0; i < 7; i++)
	{
		if (Enemies.Num() > 5)
			Selected.Add(Enemies[FMath::RandRange(0, 4)]);
		else
			Selected.Add(Enemies[FMath::RandRange(0, Enemies.Num() - 1)]);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Player;
	SpawnParams.Instigator = Player;
	int32 DaggerDamage = FMath::RandRange(4750, 5250);
	int32 DaggerSpeed = FMath::RandRange(9000, 11000);
	for (AEnemyActorBase* Target : Selected)
	{
		if (!IsValid(Target))
			continue;

		FVector Dir = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();
		FRotator SpawnRotation = Dir.Rotation();

		if (AThrowingDagger* Dagger = MeshComp->GetWorld()->SpawnActor<AThrowingDagger>(AThrowingDagger::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams))
			Dagger->InitFire(DaggerDamage, DaggerSpeed, Player);
	}
}
