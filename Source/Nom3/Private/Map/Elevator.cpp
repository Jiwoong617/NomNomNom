
  #include "Nom3/Public/Map/Elevator.h"
  #include "Components/BoxComponent.h"
  #include "Core/NomPlayer.h"

  AElevator::AElevator()
  {
        PrimaryActorTick.bCanEverTick = true;

        TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
        TriggerBox->SetupAttachment(RootComponent);

    
  }

 
  void AElevator::BeginPlay()
  {
        Super::BeginPlay();
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlap);
  }


void AElevator::Tick(float DeltaTime)
  {
      // 목표 지점 설정
      const FVector Destination = bIsTriggered ? GlobalTargetLocation : StartLocation;

      // 현재 위치와 목표 거리 계산
      const float Distance = FVector::Dist(GetActorLocation(), Destination);

      // 목표 속도 설정
      float TargetSpeed = MaxSpeed;

      // 목표 지점 근처면 감속
      if (Distance < 200.0f) 
      {
          TargetSpeed = MinSpeed;
      }

      // 보간쓰
      CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Acceleration);

      // 목표 도착 체크
      if (Distance < 2.0f) 
      {
          SetActorLocation(Destination);
          CurrentSpeed = 0.0f;
          return;
      }

      // 이동
      const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
      const FVector Movement = Direction * CurrentSpeed * DeltaTime;
      SetActorLocation(GetActorLocation() + Movement);
  }


void AElevator::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
  {
      ANomPlayer* PlayerCharacter = Cast<ANomPlayer>(OtherActor);
      if (PlayerCharacter != nullptr)
      {
          // 시작 지점이면 올라감
          if (GetActorLocation().Equals(StartLocation, 1.0f))
          {
              bIsTriggered = true;
          }
          // 목표 지점이면 내려감
          else if (GetActorLocation().Equals(GlobalTargetLocation, 1.0f))
          {
              bIsTriggered = false;
          }
      }
  }