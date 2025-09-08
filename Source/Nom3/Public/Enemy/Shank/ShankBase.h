// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShankBase.generated.h"

class UDroneMovementComponent;
class USphereComponent;
class UShankCircleStateMachine;

UENUM()
enum class EShankType : uint8
{
	ScoutShank = 0 UMETA(DisplayName = "ScoutShank"),
	SelfDestructShank = 1 UMETA(DisplayName = "SelfDestructShank"),
};

UENUM()
enum class EShankState : uint8
{
	Sleep = 0 UMETA(DisplayName = "Sleep"),
	Circle = 1 UMETA(DisplayName = "Circle"),
	Evade = 2 UMETA(DisplayName = "Evade"),
	Splash = 3 UMETA(DisplayName = "Splash"),
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
	TObjectPtr<UShankCircleStateMachine> CircleStateMachine;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetPawn;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ThrottleControlCurve;

	UPROPERTY(EditAnywhere)
	float KeepDistance;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle DodgeTimerHandle;

	UPROPERTY(VisibleAnywhere)
	EShankState CurrentState;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void OnAimByPlayerSight();

	UFUNCTION(BlueprintCallable)
	void OnShotByPlayer(FVector ShotDir, int Damage);

	UFUNCTION(BlueprintCallable)
	void OnShotDown(const FVector ShotDir);
};
