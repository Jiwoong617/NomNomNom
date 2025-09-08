// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingObject.generated.h"

// Defines the current movement state of the object
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle,
	MovingToTarget,
	MovingToStart
};

UCLASS(Abstract) // Abstract means this class cannot be placed directly in the world


class NOM3_API AMovingObject : public AActor
{
    GENERATED_BODY()

public:
	AMovingObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// The visual representation of the moving object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ObjectMesh;

	// --- Movement Properties ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Object | Movement")
	float MoveSpeed;

	// The target location in local space (relative to the object's start position)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Object | Movement", Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	// --- State ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Object | State")
	EMovementState MovementState;

protected:
	// The world-space start and target locations
	FVector StartLocation;
	FVector GlobalTargetLocation;
};
