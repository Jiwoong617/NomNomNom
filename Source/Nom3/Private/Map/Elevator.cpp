
  #include "Nom3/Public/Map/Elevator.h"
  #include "Components/BoxComponent.h"
  #include "Core/NomPlayer.h"

  // 생성자
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
        // Super::Tick(DeltaTime);

        // 목표 지점 설정
        const FVector Destination = bIsTriggered ? GlobalTargetLocation : StartLocation;

        // 목표 속도 설정
        const float TargetSpeed = FVector::Dist(GetActorLocation(), Destination) > 10.0f ? 200.0f : 0.0f; // 예시: 최고 속도를 200으로 설정

        // 보간
        float CurrentSpeed = FMath::FInterpTo(ObjectSpeed, TargetSpeed, DeltaTime, 2.0f);
        ObjectSpeed = CurrentSpeed; 

        // 속도가 거의 0이면 이동을 멈춥
        if (FMath::IsNearlyZero(ObjectSpeed))
        {
                // 위치를 한 번 더 고정
                if (!GetActorLocation().Equals(Destination, 0.5f))
                {
                        SetActorLocation(Destination);
                }
                return;
        }

        // 이동할 거리 계산
        const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
        const FVector Movement = Direction * ObjectSpeed * DeltaTime;

        //액터 이동
        SetActorLocation(GetActorLocation() + Movement);
  }


  void AElevator::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
  {
      ANomPlayer* PlayerCharacter = Cast<ANomPlayer>(OtherActor);
      if (PlayerCharacter != nullptr)
      {
          
          if (GetActorLocation().Equals(StartLocation, 1.0f))
          {
              bIsTriggered = true;
          }
         
          else if (GetActorLocation().Equals(GlobalTargetLocation, 1.0f))
          {
              bIsTriggered = false;
          }
      }
  }
