// Fill out your copyright notice in the Description page of Project Settings.

#include "Nom3/Public/Map/AutoDoor.h"
#include "Components/BoxComponent.h"
#include "Nom3/Public/Map/MovingObject.h"
#include "TimerManager.h"
#include "Core/NomPlayer.h"


AAutoDoor::AAutoDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoDoor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AAutoDoor::OnOverlapEnd);
	
	bShouldBeOpen = false;
}

void AAutoDoor::BeginPlay()
{
	Super::BeginPlay();
	// Save the max speed from the parent and initialize current speed to 0
	StartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	
	if (StartLocation.IsNearlyZero())
	{
		StartLocation = GetActorLocation();
	}
	if (GlobalTargetLocation.IsNearlyZero())
	{
		GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	}
	MaxSpeed = ObjectSpeed;
	ObjectSpeed = 0.0f;
}

void AAutoDoor::Tick(float DeltaTime)
{
	// Super::Tick(DeltaTime);

	// This movement logic is adapted from your AElevator class for consistency.

	// 목표 지점 설정
	FVector Destination = bShouldBeOpen ? GlobalTargetLocation : StartLocation;
	float DistanceToDestination = FVector::Dist(GetActorLocation(), Destination);

	// 목표 속도 설정 (거리가 1.0f보다 크면 MaxSpeed, 아니면 0)
	float TargetSpeed = (DistanceToDestination > 1.0f) ? MaxSpeed : 0.0f;

	// 보간을 사용하여 현재 속도 조절
	ObjectSpeed = FMath::FInterpTo(ObjectSpeed, TargetSpeed, DeltaTime, Acceleration);

	// 속도가 거의 0이고, 목표 속도도 0이면 (즉, 멈춰야 하는 상황이면)
	if (FMath::IsNearlyZero(ObjectSpeed) && FMath::IsNearlyZero(TargetSpeed))
	{
		// 아직 목표 지점에 정확히 도달하지 않았다면 위치를 맞춰줍니다.
		if (DistanceToDestination > 0.0f)
		{
			SetActorLocation(Destination);
		}
		ObjectSpeed = 0.0f; // 속도를 확실하게 0으로 설정
		return; // 더 이상 움직일 필요가 없으므로 함수 종료
	}

	// 이동 및 방향 구현
	FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	FVector Movement = Direction * ObjectSpeed * DeltaTime;

	// 이번 프레임의 이동량이 남은 거리를 초과하는 경우 (Overshooting 방지)
	if (Movement.Size() >= DistanceToDestination)
	{
		SetActorLocation(Destination);
		ObjectSpeed = 0.0f; // 도착했으므로 속도를 0으로 설정
	}
	else
	{
		SetActorLocation(GetActorLocation() + Movement);
	}
}

void AAutoDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ANomPlayer>(OtherActor); 
	 if (PlayerCharacter != nullptr)
	 {
	
	 	GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
		bShouldBeOpen = true;
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 문을 열었습니다."));
	 }
}

void AAutoDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	 ACharacter* PlayerCharacter = Cast<ANomPlayer>(OtherActor); 
	 if (PlayerCharacter != nullptr)
	 {
		// When the player leaves, start a timer to close the door.
	 	GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, this, &AAutoDoor::CloseDoor, CloseDelay, false);
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 지나갔으니 문을 닫습니다."));
	 }
}

void AAutoDoor::CloseDoor()
{
	bShouldBeOpen = false;
	UE_LOG(LogTemp, Warning, TEXT("시간이 다 되었으니 문을 닫습니다."));
}
