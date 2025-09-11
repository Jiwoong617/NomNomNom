// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HandCannon.generated.h"

UCLASS()
class NOM3_API AHandCannon : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHandCannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NoAimFire() override;
	virtual void AimFire() override;
};
