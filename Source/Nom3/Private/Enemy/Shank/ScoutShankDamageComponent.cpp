// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShankDamageComponent.h"
#include "Enemy/Shank/ScoutShank.h"
#include "Kismet/GameplayStatics.h"

UScoutShankDamageComponent::UScoutShankDamageComponent()
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

void UScoutShankDamageComponent::OnHitBody(FFireInfo& Info)
{
	Super::OnHitBody(Info);

	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());
	
	if (const auto OwnerShank = Cast<AShankBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerShank->OnDamaged(Info);

		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerShank->GetName()));
	}
}