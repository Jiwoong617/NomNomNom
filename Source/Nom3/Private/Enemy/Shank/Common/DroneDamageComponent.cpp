// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/DroneDamageComponent.h"
#include "Enemy/Shank/Common/DroneBase.h"
#include "Kismet/GameplayStatics.h"

UDroneDamageComponent::UDroneDamageComponent()
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

void UDroneDamageComponent::OnHitBody(FFireInfo& Info)
{
	Super::OnHitBody(Info);

	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());
	
	if (const auto OwnerDrone = Cast<ADroneBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerDrone->OnDamaged(Info);

		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerDrone->GetName()));
	}
}

void UDroneDamageComponent::OnHitHead(FFireInfo& Info)
{
	Super::OnHitHead(Info);

	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());

	if (const auto OwnerDrone = Cast<ADroneBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerDrone->OnCriticalDamaged(Info);

		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerDrone->GetName()));
	}
}