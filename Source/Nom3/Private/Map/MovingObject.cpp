// Fill out your copyright notice in the Description page of Project Settings.


<<<<<<< HEAD

#include "Nom3/Public/Map/MovingObject.h"
#include "Components/StaticMeshComponent.h"

=======
#include  "Nom3/Public/Map/MovingObject.h"
#include "Components/StaticMeshComponent.h"
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)

// Sets default values
AMovingObject::AMovingObject()
{

	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	SetRootComponent(ObjectMesh);

<<<<<<< HEAD
	MoveSpeed = 200.0f;
	TargetLocation = FVector(0.0f, 0.0f, 500.0f);
	MovementState = EMovementState::Idle;
=======
	ObjectSpeed = 100.0f;
	bIsTriggered = false;
	TargetLocation = FVector(0.0f, 0.0f, 200.0f);
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();
<<<<<<< HEAD


=======
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
	StartLocation = GetActorLocation();
    
	//정우 형이 말한 설정한 위치 월드좌표계의 위치를 변환화여 저장하는 형식
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

<<<<<<< HEAD

	if (MovementState == EMovementState::Idle)
	{
		return;
	}
=======
	//true 면 지정 위치 false 시작위치
	FVector Destination = bIsTriggered ? GlobalTargetLocation : StartLocation;
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)

	// 방향 구하기
	FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();

<<<<<<< HEAD

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
=======
	// 현재 위치와 목표 지점 사이의 거리
	float DistanceToDestination = FVector::Dist(GetActorLocation(), Destination);

	// 목표 지점에 거의 도달했다면 이동을 멈춥니다. (정확히 위치를 고정시켜 떨림 방지)
	if (DistanceToDestination < 1.0f)
	{
		SetActorLocation(Destination);
		return;
	}

	// 이동
	FVector Movement = Direction * ObjectSpeed * DeltaTime;
	SetActorLocation(GetActorLocation() + Movement);
}
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
