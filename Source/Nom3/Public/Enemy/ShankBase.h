// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShankBase.generated.h"

class UDroneMovementComponent;
class USphereComponent;

UENUM()
enum EShankType
{
	ScoutShank = 0 UMETA(DisplayName = "ScoutShank"),
	SelfDestructShank = 1 UMETA(DisplayName = "SelfDestructShank"),
};

UCLASS()
class NOM3_API AShankBase : public AActor
{
	GENERATED_BODY()

public:
	AShankBase();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> NormalHitComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CriticalHitComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneMovementComponent> DroneMoveComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ThrottleControlCurve;

	UPROPERTY(EditAnywhere)
	float KeepDistance;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle DodgeTimerHandle;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DecideTargetLocation();

	UFUNCTION(BlueprintCallable)
	void OnAimByPlayerSight();

	UFUNCTION(BlueprintCallable)
	void OnShotByPlayer(int Damage);

	void OnShotDown();

};
