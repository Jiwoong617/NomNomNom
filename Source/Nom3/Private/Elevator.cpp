// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h" // 플레이어 캐릭터 확인을 위해 추가

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnOverlapEnd);
	
	MinSpeed = 500.0f;
	Acceleration = 10.0f;
	// 이거 처음에 안움직이게 할려고 한거임 재민아 까먹지 말아라
	bIsTriggered = false;
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	MaxSpeed = ObjectSpeed;
	ObjectSpeed = 0.0f;
	
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	// Super::Tick(DeltaTime); 상속된 부모 틱 안쓸꺼라서 주석 처리함 gpt 가 이렇게 하래요
	
	//목표 지점 설정
	FVector Destination = bIsTriggered ? GlobalTargetLocation : StartLocation;
    
	//목표 속도 설정 
	float TargetSpeed = FVector::Dist(GetActorLocation(), Destination) > 1.0f ? MaxSpeed : 0.0f;
    
	//현재 속도에서 목표 속도로 부드럽게 변경
	ObjectSpeed = FMath::FInterpTo(ObjectSpeed, TargetSpeed, DeltaTime, Acceleration);

	//속도가 0에 가까우면 더 이상 계산하지 않음
	if (FMath::IsNearlyZero(ObjectSpeed))
	{
		return;
	}

	//이동 방향과 거리 계산
	FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	FVector Movement = Direction * ObjectSpeed * DeltaTime;
    
	//실제로 액터 이동
	SetActorLocation(GetActorLocation() + Movement);
	
}
void AElevator::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	// if (PlayerCharacter != nullptr)
	// {
		// bIsTriggered 상태를 반전시켜 엘리베이터의 목표 위치를 변경합니다.
		// true이면 위(GlobalTargetLocation)로, false이면 아래(StartLocation)로 이동합니다.
		bIsTriggered = !bIsTriggered;
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 감지되었씁니다 현재 트리거는: %s 입니다"), bIsTriggered ? TEXT("true") : TEXT("false"));
	// }
}

void AElevator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	return;
}