// Fill out your copyright notice in the Description page of Project Settings.


#include "Nom3/Public/Map/MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WaitTime = 2.0f;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//목표 지점으로
	bIsTriggered = true; 
    
	//원래 속도 저장
	OriginalSpeed = ObjectSpeed;
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//타이머 활성화 중일떈 정지 하도록
	if (GetWorld()->GetTimerManager().IsTimerActive(MovementTimerHandle))
	{
		return;
	}

	// 현재 위치 목표지점 간의 거리
	FVector Destination = bIsTriggered ? GlobalTargetLocation : StartLocation;
	float Distance = FVector::Dist(GetActorLocation(), Destination);

	// 목표 지점에 도착했다면
	if (Distance < 1.0f) // 작다면
	{
		// 정지
		ObjectSpeed = 0.0f;

		//2 초뒤 toggle_movement 실행
		GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle,this,  &AMovingPlatform::ToggleMovement,WaitTime,false);
	}
}

void AMovingPlatform::ToggleMovement()
{
	//이동방향을 반대로 바꿈
	bIsTriggered = !bIsTriggered;

	//아까 멈춘거 원래 속도로
	ObjectSpeed = OriginalSpeed;
    
	//움직이면서 다시 타이머 초기화
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	}
}

