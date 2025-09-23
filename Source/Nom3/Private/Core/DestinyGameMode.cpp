// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DestinyGameMode.h"

#include "Core/NomPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Nom3/Public/Core/DestinyPlayerController.h"

void ADestinyGameMode::OnServitorDied(AServitor* KilledServitor)
{
	ADestinyPlayerController* PC = Cast<ADestinyPlayerController>( UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->ShowClearUI();
		if (ANomPlayer* p = Cast<ANomPlayer>(UGameplayStatics::GetPlayerPawn(this, 0)))
			p->OnGameCleared();
	}
}
