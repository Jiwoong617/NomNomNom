// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DestinyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Nom3/Public/Core/DestinyPlayerController.h"
#include "Servitor.generated.h"

void ADestinyGameMode::OnServitorDied(AServitor* KilledServitor)
{
	ADestinyPlayerController* PC = Cast<ADestinyPlayerController>( UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->ShowClearUI();
	}
}
