#include "Map/Elevator.h"

void AElevator::Interact_Implementation(AActor* Interactor)
{
	// 움직이고있으면 안움직임
	if (MovementState != EMovementState::Idle)
	{
		return;
	}

	// 시작위치라면 목표로
	if (GetActorLocation().Equals(StartLocation, 1.0f))
	{
		MovementState = EMovementState::MovingToTarget;
	}
	// 목표라면 시작 위치로
	else if (GetActorLocation().Equals(GlobalTargetLocation, 1.0f))
	{
		MovementState = EMovementState::MovingToStart;
	}
}