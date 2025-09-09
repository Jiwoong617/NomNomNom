// MovingPlatform.cpp

#include "Map/MovingPlatform.h"

void AMovingPlaform::BeginPlay()
{
	Super::BeginPlay();

	
	MovementState = EMovementState::MovingToTarget;
}

void AMovingPlaform::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	
	if (MovementState == EMovementState::Idle)
	{
		
		if (GetActorLocation().Equals(GlobalTargetLocation, 1.0f))
		{
			GetWorldTimerManager().SetTimer(MovementHandle,AMovingPlaform::Movementdelay,true);
			MovementState = EMovementState::MovingToStart;
		}
		
		else if (GetActorLocation().Equals(StartLocation, 1.0f))
		{
			GetWorldTimerManager().SetTimer(MovementHandle,AMovingPlaform::Movementdelay,true);
			MovementState = EMovementState::MovingToTarget;
		}
	}
}