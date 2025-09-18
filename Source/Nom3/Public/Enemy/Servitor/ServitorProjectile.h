// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/ProjectileBase.h"
#include "ServitorProjectile.generated.h"

UCLASS()
class NOM3_API AServitorProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AServitorProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
