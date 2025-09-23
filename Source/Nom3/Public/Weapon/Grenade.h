// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class UNiagaraSystem;
class ANomPlayer;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USoundWave;

UCLASS()
class NOM3_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float ExplodeRadius = 500.f;
	float MinDamage = 1000.f;
	float MaxDamage = 2000.f;
	
	UPROPERTY(EditAnywhere)
	ANomPlayer* Player;
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplodeEff;

private:
	UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION() void OnExplode();
public:
	UFUNCTION() void Init(FVector Direction, float Speed, ANomPlayer* NomPlayer);
};
