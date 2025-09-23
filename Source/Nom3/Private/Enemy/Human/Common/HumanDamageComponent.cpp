// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanDamageComponent.h"
#include "Enemy/Core/EnemyCharacterBase.h"
#include "Enemy/Human/Common/HumanBase.h"
#include "Kismet/GameplayStatics.h"

UHumanDamageComponent::UHumanDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//총알 명중 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/Dreg/Sound/SC_DregBulletHit.SC_DregBulletHit"));
		Finder.Succeeded())
	{
		HitSound = Finder.Object;
	}
}

void UHumanDamageComponent::OnHitBody(FFireInfo& Info)
{
	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());

	if (const auto OwnerHuman = Cast<AHumanBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerHuman->OnDamaged(Info);

		//디버그
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerHuman->GetActorLabel()));
	}
}

void UHumanDamageComponent::OnHitHead(FFireInfo& Info)
{
	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());

	if (const auto OwnerHuman = Cast<AHumanBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerHuman->OnCriticalDamaged(Info);

		//디버그
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerHuman->GetActorLabel()));
	}
}