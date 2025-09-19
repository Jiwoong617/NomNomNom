// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DestinyGameMode.generated.h"

/**
 * 
 */
// 전방선언
class AServitor;
UCLASS()
class NOM3_API ADestinyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	void OnServitorDied(AServitor* Killed);
};
