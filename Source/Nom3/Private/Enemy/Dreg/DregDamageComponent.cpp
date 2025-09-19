// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Dreg/DregDamageComponent.h"

#include "Enemy/Core/EnemyCharacterBase.h"
#include "Kismet/GameplayStatics.h"

UDregDamageComponent::UDregDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDregDamageComponent::OnHitBody(FFireInfo& Info)
{
	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());

	if (const auto OwnerEnemy = Cast<AEnemyCharacterBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerEnemy->OnDamaged(Info);

		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerEnemy->GetName()));
	}
}

void UDregDamageComponent::OnHitHead(FFireInfo& Info)
{
	Super::OnHitHead(Info);

	//총알 명중 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetOwner()->GetActorLocation());

	if (const auto OwnerEnemy = Cast<AEnemyCharacterBase>(GetOwner()))
	{
		//데미지 적용 시도
		OwnerEnemy->OnCriticalDamaged(Info);

		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, *FString::Printf(TEXT("%s is Hit!"), *OwnerEnemy->GetName()));
	}
}