
#include "Map/Elevator.h" 
#include "Components/BoxComponent.h"
#include "Core/NomPlayer.h" 
#include "TimerManager.h"

 AElevator::AElevator()
  {
      PrimaryActorTick.bCanEverTick = true;

      // 루트 컴포넌트가 없으면 생성
      if (!RootComponent)
      {
          RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
      }

      TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
      TriggerBox->SetupAttachment(RootComponent);
  }

  void AElevator::BeginPlay()
  {
      Super::BeginPlay();
      TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlap);

      // 시작 위치를 현재 액터의 위치로 설정
      StartLocation = GetActorLocation();
     
  }

  void AElevator::Tick(float DeltaTime)
  {
      Super::Tick(DeltaTime);

      FVector Destination;
      bool bShouldMove = false;

      // 현재 상태에 따라 목적지와 행동을 결정
      switch (CurrentState)
      {
          case EElevatorState::MovingToMidpoint:
              Destination = MidpointLocation;
              bShouldMove = true;
              // 중간 지점 도착 체크
              if (GetActorLocation().Equals(Destination, 1.0f))
              {
                  CurrentState = EElevatorState::WaitingAtMidpoint;
                  bShouldMove = false;
                  // 15초 대기
                  GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AElevator::ResumeMovementToFinalTarget, WaitTimeAtMidpoint, false);
              }
              break;

          case EElevatorState::MovingToFinalTarget:
              Destination = GlobalTargetLocation;
              bShouldMove = true;
              // 최종 목적지 도착 체크
              if (GetActorLocation().Equals(Destination, 1.0f))
              {
                  CurrentState = EElevatorState::IdleAtFinalTarget;
                  bShouldMove = false;
              }
              break;

          case EElevatorState::ReturningToStart:
              Destination = StartLocation;
              bShouldMove = true;
              // 시작 지점 도착 체크
              if (GetActorLocation().Equals(Destination, 1.0f))
              {
                  CurrentState = EElevatorState::IdleAtStart;
                  bShouldMove = false;
              }
              break;

          default:
          
              bShouldMove = false;
              break;
      }

      if (bShouldMove)
      {
          const float Distance = FVector::Dist(GetActorLocation(), Destination);
          float TargetSpeed = MaxSpeed;

          if (Distance < 200.0f)
          {
              TargetSpeed = MinSpeed;
          }

          CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Acceleration);

          if (Distance < 2.0f)
          {
              SetActorLocation(Destination);
              CurrentSpeed = 0.0f;
              return;
          }

          const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
          const FVector Movement = Direction *  CurrentSpeed * DeltaTime;
          SetActorLocation(GetActorLocation() + Movement);
      }
      else
      {
          // 움직이지 않을 때는 속도를 0으로 줄임
          CurrentSpeed = FMath::FInterpTo(CurrentSpeed, 0.0f, DeltaTime, Acceleration);
      }
  }

  void AElevator::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
  bFromSweep, const FHitResult& SweepResult)
  {
      if (Cast<ANomPlayer>(OtherActor) != nullptr)
      {
          
          if (CurrentState == EElevatorState::IdleAtStart)
          {
              CurrentState = EElevatorState::MovingToMidpoint;
          }
         
          else if (CurrentState == EElevatorState::IdleAtFinalTarget)
          {
              CurrentState = EElevatorState::ReturningToStart;
          }
      }
  }

  void AElevator::ResumeMovementToFinalTarget()
  {
      // 최종 목적
      CurrentState = EElevatorState::MovingToFinalTarget;
  }