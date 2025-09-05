// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/DroneMovementComponent.h"

// Sets default values for this component's properties
UDroneMovementComponent::UDroneMovementComponent() :
	bSimulate(false),
	GravityForce(0),
	GravityDirection(FVector::DownVector),
	CruiseThrustForce(500),
	ThrustForce(500),
	ThrustDirection(FVector::ForwardVector),
	ThrustVectoringLevel(0.75),
	LaunchSpeed(0),
	Bounciness(0.5),
	DragCoefficient(5),
	Mass(10)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDroneMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDroneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//시뮬레이션 활성화 여부
	if (bSimulate == false)
	{
		return;
	}

	//중력 적용 (등가 원리)
	Velocity += GravityForce * GravityDirection * DeltaTime;

	//추진력 적용
	Velocity += (ThrustForce * ThrustDirection / Mass) * DeltaTime;
	
	//공기 저항 적용
	const FVector DragForce = -Velocity * DragCoefficient;
	const FVector DragAcceleration = DragForce / Mass;
	Velocity += DragAcceleration * DeltaTime;
	
	//이동량 연산
	const FVector Delta = Velocity * DeltaTime;

	//속도의 방향을 드로우
	const FVector Location = GetOwner()->GetActorLocation();
	DrawDebugLine(GetWorld(), Location, Location + Velocity, FColor::Red, false, -1, 0, 0 );

	//안전 이동 이동
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true, Hit);

	//충돌 발생
	if (Hit.bBlockingHit)
	{
		//튕겨나감
		Velocity = Velocity.MirrorByVector(Hit.ImpactNormal) * Bounciness;
	}
}

void UDroneMovementComponent::Launch(const FVector LaunchDirection)
{
	//사출 속도 결정
	Velocity = LaunchDirection * LaunchSpeed;

	//시뮬레이션 활성화
	bSimulate = true;
}

void UDroneMovementComponent::Shock(const FVector Direction, const float Magnitude)
{
	//충격 적용
	const FVector Impulse = Direction * Magnitude;
	Velocity += Impulse / Mass;
}

void UDroneMovementComponent::ThrottleThrustByLevel(const float Level)
{
	//목표 추진력 결정
	ThrustForce = CruiseThrustForce * Level;
}

void UDroneMovementComponent::ThrottleToCruiseThrust()
{
	//순항 추진력을 향해 점진적 보간
	ThrustForce = FMath::Lerp(ThrustForce, CruiseThrustForce, 0.25);
}

void UDroneMovementComponent::ThrottleHighToEvade()
{
	ThrottleThrustByLevel(30);
}

void UDroneMovementComponent::VectorThrust(const FVector Direction)
{
	//추진 방향 구면 보간
	ThrustDirection = FVector::SlerpVectorToDirection(ThrustDirection, Direction, ThrustVectoringLevel);
}

void UDroneMovementComponent::Fall()
{
	//추진력 상실
	ThrustForce = 0;

	//중력 가속 시작
	GravityForce = 980;
}

void UDroneMovementComponent::Impulse()
{
	//충격 발생
	Velocity += FMath::RandRange(400, 800) * FMath::VRand();
}