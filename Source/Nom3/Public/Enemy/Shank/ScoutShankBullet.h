// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/ProjectileBase.h"
#include "ScoutShankBullet.generated.h"

UCLASS()
class NOM3_API AScoutShankBullet : public AProjectileBase
{
	GENERATED_BODY()

public:
	AScoutShankBullet();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

public:
	virtual void Tick(float DeltaTime) override;
};
