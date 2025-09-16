// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/ShankBase.h"
#include "SelfDestructShank.generated.h"

UCLASS()
class NOM3_API ASelfDestructShank : public AShankBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelfDestructShank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
