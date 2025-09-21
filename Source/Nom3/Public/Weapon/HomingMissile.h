// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/ProjectileBase.h"
#include "HomingMissile.generated.h"

class ANomPlayer;

UCLASS()
class NOM3_API AHomingMissile : public AActor
{
	GENERATED_BODY()

public:
    AHomingMissile();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere) UStaticMeshComponent* MeshComp;
    UPROPERTY(EditAnywhere) USphereComponent* SphereComp;
    UPROPERTY(EditAnywhere) UProjectileMovementComponent* ProjectileMoveComp;

	FTimerHandle CantReachTimer;
    
    UPROPERTY(EditAnywhere)
    ANomPlayer* Player;

    UPROPERTY(EditAnywhere, Category="HomingMissile")
    int32 Damage;
    UPROPERTY(EditAnywhere, Category="HomingMissile")
    float Speed = 3000.f;
    UPROPERTY(EditAnywhere, Category="HomingMissile")
    float HomingDelay = 0.5f;
    UPROPERTY(EditAnywhere, Category="HomingMissile")
    float HomingStrength = 20000.f;
    UPROPERTY(EditAnywhere, Category="HomingMissile")
    float StraightFlightExplodeDelay = 2.0f;
    UPROPERTY(EditAnywhere, Category="HomingMissile")
    float ExplosionRadius = 300.f;

    void SetHoming();
    void Explode();

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    void InitFire(int32 Dmg, ANomPlayer* player);
};
