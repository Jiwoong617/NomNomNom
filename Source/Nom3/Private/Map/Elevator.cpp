#include "Map/Elevator.h"
#include "Components/BoxComponent.h"
#include "Core/NomPlayer.h"
#include "TimerManager.h"

// 로그 카테고리를 정의합니다. (파일 상단에 추가)
DEFINE_LOG_CATEGORY_STATIC(LogElevator, Log, All);

AElevator::AElevator()
{
    // ... (생성자 내용은 이전과 동일)
    PrimaryActorTick.bCanEverTick = true;
    if (!RootComponent) RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
}

void AElevator::BeginPlay()
{
    Super::BeginPlay();
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlap);
    StartLocation = GetActorLocation();
    UE_LOG(LogElevator, Warning, TEXT("엘리베이터 시작. 현재 상태: IdleAtStart"));
}

void AElevator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Destination;
    bool bShouldMove = false;
    
    switch (CurrentState)
    {
        case EElevatorState::MovingToFirstTarget:
            Destination = FirstTargetLocation;
            bShouldMove = true;
            break;
        case EElevatorState::MovingToSecondTarget:
            Destination = SecondTargetLocation;
            bShouldMove = true;
            break;
        default:
            bShouldMove = false;
            break;
    }

    if (bShouldMove)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector TargetLocation = FVector(CurrentLocation.X, CurrentLocation.Y, Destination.Z);
        

        
        const float Distance = FMath::Abs(CurrentLocation.Z - TargetLocation.Z);
        float TargetSpeed = MaxSpeed;
        if (Distance < 200.0f) TargetSpeed = MinSpeed;
        
        CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Acceleration);
        const FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
        const FVector Movement = Direction * CurrentSpeed * DeltaTime;

        if (Movement.SizeSquared() >= FVector::DistSquared(CurrentLocation, TargetLocation))
        {
            SetActorLocation(TargetLocation);
            CurrentSpeed = 0.0f;
            
            switch (CurrentState)
            {
                case EElevatorState::MovingToFirstTarget:
                    CurrentState = EElevatorState::WaitingAtFirstTarget;
                    UE_LOG(LogElevator, Warning, TEXT("첫 번째 목적지 도착. 15초 타이머 시작. 현재 상태: WaitingAtFirstTarget"));
                    GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AElevator::MoveToSecondTarget, HowToDelay, false);
                    break;
                case EElevatorState::MovingToSecondTarget:
                    CurrentState = EElevatorState::IdleAtSecondTarget;
                    UE_LOG(LogElevator, Warning, TEXT("두 번째 목적지 도착. 영구 정지. 현재 상태: IdleAtSecondTarget"));
                    break;
            }
        }
        else
        {
            SetActorLocation(CurrentLocation + Movement);
        }
    }
    else
    {
        CurrentSpeed = FMath::FInterpTo(CurrentSpeed, 0.0f, DeltaTime, Acceleration);
    }
}

void AElevator::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<ANomPlayer>(OtherActor) != nullptr)
    {
        if (CurrentState == EElevatorState::IdleAtStart)
        {
            CurrentState = EElevatorState::MovingToFirstTarget;
            UE_LOG(LogElevator, Warning, TEXT("플레이어 감지. 첫 번째 목적지로 이동 시작. 현재 상태: MovingToFirstTarget"));
        }
    }
}

void AElevator::MoveToSecondTarget()
{
    UE_LOG(LogElevator, Warning, TEXT("타이머 완료. 두 번째 목적지로 이동 시작. 현재 상태: MovingToSecondTarget"));
    CurrentState = EElevatorState::MovingToSecondTarget;
}