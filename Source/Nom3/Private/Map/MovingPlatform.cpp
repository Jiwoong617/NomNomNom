// MovingPlatform.cpp

#include "Map/MovingPlatform.h"

void AMovingPlaform::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면 즉시 목표 지점으로 이동을 시작합니다.
	MovementState = EMovementState::MovingToTarget;
}

void AMovingPlaform::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 함수를 호출하여 실제 이동을 처리합니다.
	Super::Tick(DeltaTime);

	// 이동이 끝나서 상태가 Idle이 되었는지 확인합니다.
	if (MovementState == EMovementState::Idle)
	{
		// 현재 위치가 목표 지점과 거의 같다면, 시작 지점으로 돌아가도록 상태를 변경합니다.
		// 부동 소수점 오차를 감안하여 Equals 함수로 비교합니다.
		if (GetActorLocation().Equals(GlobalTargetLocation, 1.0f))
		{
			MovementState = EMovementState::MovingToStart;
		}
		// 현재 위치가 시작 지점과 거의 같다면, 다시 목표 지점으로 이동하도록 상태를 변경합니다.
		else if (GetActorLocation().Equals(StartLocation, 1.0f))
		{
			MovementState = EMovementState::MovingToTarget;
		}
	}
}