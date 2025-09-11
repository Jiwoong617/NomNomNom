// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

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
	UPROPERTY(EditAnywhere)
	bool IsHit = false;

	float ExplodeRadius = 500.f;
	float MinDamage = 100.f;
	float MaxDamage = 1000.f;
	
	//UPROPERTY(EditAnywhere)
	//UProjectileMovementComponent* Projectile;
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	FTimerHandle TimerHandle;
	
private:
	UFUNCTION() void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION() void OnExplode();
public:
	UFUNCTION() void Init(FVector Direction, float Speed);
};
