// Fill out your copyright notice in the Description page of Project Settings.

#include "Nom3/Public/Map/AutoDoor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

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
	MaxSpeed = ObjectSpeed;
	ObjectSpeed = 0.0f;
}

void AAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This movement logic is adapted from your AElevator class for consistency.

	//목표 지점 설정
	FVector Destination = bShouldBeOpen ? GlobalTargetLocation : StartLocation;
	float TargetSpeed = FVector::Dist(GetActorLocation(), Destination) > 1.0f ? MaxSpeed : 0.0f;
    
	// 보간쓰
	ObjectSpeed = FMath::FInterpTo(ObjectSpeed, TargetSpeed, DeltaTime, Acceleration);

	if (FMath::IsNearlyZero(ObjectSpeed))
	{
		return;
	}

	//이동 및 방향 구현
	FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	FVector Movement = Direction * ObjectSpeed * DeltaTime;
    
	SetActorLocation(GetActorLocation() + Movement);
}

void AAutoDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor); 전과 동일 알아서 바꾸셈
	// if (PlayerCharacter != nullptr)
	// {
	// 	// When the player enters, cancel any pending close timer and open the door.
	// 	GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
		bShouldBeOpen = true;
		UE_LOG(LogTemp, Warning, TEXT("플레이가 문을 열었습니다."));
	// }
}

void AAutoDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor); 위와 동일
	// if (PlayerCharacter != nullptr)
	// {
	// 	// When the player leaves, start a timer to close the door.
	// 	GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, this, &AAutoDoor::CloseDoor, CloseDelay, false);
		UE_LOG(LogTemp, Warning, TEXT("플레이거 지나갔으니 문을 닫습니다."));
	// }
}

void AAutoDoor::CloseDoor()
{
	bShouldBeOpen = false;
	UE_LOG(LogTemp, Warning, TEXT("시간이 다 되었으니 문을 닫습니다."));
}
