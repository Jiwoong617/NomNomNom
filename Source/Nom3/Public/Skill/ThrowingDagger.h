// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ThrowingDagger.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class NOM3_API AThrowingDagger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AThrowingDagger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere) USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere) UProjectileMovementComponent* ProjectileMoveComp;

	UPROPERTY()
	FFireInfo FireInfo;
	
	UPROPERTY(EditAnywhere)
	AActor* Player;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void InitFire(int32 Dmg, int32 Speed, AActor* player);
};
