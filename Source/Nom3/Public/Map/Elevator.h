
 #pragma once

  #include "CoreMinimal.h"
  #include "GameFramework/Actor.h"
  #include "Elevator.generated.h"

  class UBoxComponent;
  class ANomPlayer;


  UENUM(BlueprintType)
  enum class EElevatorState : uint8
  {
      IdleAtStart,        
      MovingToMidpoint,    
      WaitingAtMidpoint,   
      MovingToFinalTarget, 
      IdleAtFinalTarget,   
      ReturningToStart     
  };

  UCLASS()
  class NOM3_API AElevator : public AActor
  {
      GENERATED_BODY()

  public:
      AElevator();

  protected:
      virtual void BeginPlay() override;
      virtual void Tick(float DeltaTime) override;

      UFUNCTION()
      void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
  const FHitResult& SweepResult);

   
      void ResumeMovementToFinalTarget();

  private:
      // 컴포넌트
      UPROPERTY(VisibleAnywhere)
      UBoxComponent* TriggerBox;

      // 이동 관련 변수
      UPROPERTY(EditAnywhere, Category = "Elevator Movement", meta = (MakeEditWidget = "true"))
      FVector StartLocation;

      UPROPERTY(EditAnywhere, Category = "Elevator Movement", meta = (MakeEditWidget = "true"))
      FVector MidpointLocation; 

      UPROPERTY(EditAnywhere, Category = "Elevator Movement", meta = (MakeEditWidget = "true"))
      FVector GlobalTargetLocation;

      UPROPERTY(EditAnywhere, Category = "Elevator Movement")
      float MinSpeed = 100.0f;

      UPROPERTY(EditAnywhere, Category = "Elevator Movement")
      float MaxSpeed = 500.0f;

      UPROPERTY(EditAnywhere, Category = "Elevator Movement")
      float Acceleration = 1.0f;

      UPROPERTY(EditAnywhere, Category = "Elevator Movement")
      float WaitTimeAtMidpoint = 15.0f; 

      float CurrentSpeed = 0.0f;

      // 상태 관리 변수
      EElevatorState CurrentState = EElevatorState::IdleAtStart; 
      FTimerHandle WaitTimerHandle;
  };