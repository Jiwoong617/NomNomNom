// Fill out your copyright notice in the Description page of Project Settings.



#include "Nom3/Public/Map/MovingObject.h"
#include "Components/StaticMeshComponent.h"


AMovingObject::AMovingObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	SetRootComponent(ObjectMesh);

	MoveSpeed = 200.0f;
	TargetLocation = FVector(0.0f, 0.0f, 500.0f);
	MovementState = EMovementState::Idle;
}

void AMovingObject::BeginPlay()
{
	Super::BeginPlay();


	StartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (MovementState == EMovementState::Idle)
	{
		return;
	}

	const FVector Destination = (MovementState == EMovementState::MovingToTarget) ? GlobalTargetLocation : StartLocation;
    const float DistanceToDestination = FVector::Dist(GetActorLocation(), Destination);


	if (DistanceToDestination < 1.0f)
	{
		SetActorLocation(Destination, false);
		MovementState = (MovementState == EMovementState::MovingToTarget) ? EMovementState::MovingToStart : EMovementState::MovingToTarget;
		return;
	}

    
    const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
    const FVector MovementThisFrame = Direction * MoveSpeed * DeltaTime;

 
    if (MovementThisFrame.Size() >= DistanceToDestination)
    {
        SetActorLocation(Destination, false);
        MovementState = (MovementState == EMovementState::MovingToTarget) ? EMovementState::MovingToStart : EMovementState::MovingToTarget;
    }
    else
    {
        
	    SetActorLocation(GetActorLocation() + MovementThisFrame, false);
    }
}
