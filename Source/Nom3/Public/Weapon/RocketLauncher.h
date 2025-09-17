// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RocketLauncher.generated.h"

class AHomingMissile;

UCLASS()
class NOM3_API ARocketLauncher : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARocketLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	class
	TSubclassOf<AHomingMissile> Missile;

	FTimerHandle FireTimer;
	int32 FireCounter = 0;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AimFire() override;
	virtual void NoAimFire() override;
};
