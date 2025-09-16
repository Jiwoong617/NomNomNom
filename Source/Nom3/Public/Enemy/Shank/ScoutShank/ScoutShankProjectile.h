// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/ProjectileBase.h"
#include "ScoutShankProjectile.generated.h"

UCLASS()
class NOM3_API AScoutShankProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AScoutShankProjectile();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
