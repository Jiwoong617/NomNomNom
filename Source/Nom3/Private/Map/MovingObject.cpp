// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "Nom3/Public/Map/MovingObject.h"


AMovingObject::AMovingObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	SetRootComponent(ObjectMesh);

	// Default values
	MoveSpeed = 200.0f;
	TargetLocation = FVector(0.0f, 0.0f, 500.0f); // Default to moving 5m up
	MovementState = EMovementState::Idle;
}

void AMovingObject::BeginPlay()
{
	Super::BeginPlay();

	// Store the initial start and target locations in world space
	StartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If we are idle, there's nothing to do.
	if (MovementState == EMovementState::Idle)
	{
		return;
	}

	const FVector Destination = (MovementState == EMovementState::MovingToTarget) ? GlobalTargetLocation : StartLocation;
    const float DistanceToDestination = FVector::Dist(GetActorLocation(), Destination);

	// If we are very close, just snap to the destination and stop.
	if (DistanceToDestination < 1.0f)
	{
		SetActorLocation(Destination, true);
		MovementState = EMovementState::Idle;
		return;
	}

    // Calculate the movement vector for this frame
    const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
    const FVector MovementThisFrame = Direction * MoveSpeed * DeltaTime;

    // Check if this movement would overshoot the destination.
    if (MovementThisFrame.Size() >= DistanceToDestination)
    {
        // If so, just move directly to the destination and stop.
        SetActorLocation(Destination, true);
        MovementState = EMovementState::Idle;
    }
    else
    {
        // Otherwise, perform the movement.
	    SetActorLocation(GetActorLocation() + MovementThisFrame, true);
    }
}
