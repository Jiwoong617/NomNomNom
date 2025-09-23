#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

// 엘리베이터의 현재 상태를 정의하는 열거형(Enum)입니다.
UENUM(BlueprintType)
enum class EElevatorState : uint8
{
    IdleAtStart         UMETA(DisplayName = "Start Point Idle"),
    MovingToFirstTarget UMETA(DisplayName = "Moving to First Target"),
    WaitingAtFirstTarget UMETA(DisplayName = "Waiting at First Target"),
    MovingToSecondTarget UMETA(DisplayName = "Moving to Second Target"),
    IdleAtSecondTarget  UMETA(DisplayName = "End Point Idle")
};

UCLASS()
class NOM3_API AElevator : public AActor
{
    GENERATED_BODY()

public:
    // 생성자
    AElevator();

    // 매 프레임 호출되는 함수
    virtual void Tick(float DeltaTime) override;

protected:
    // 게임 시작 시 호출되는 함수
    virtual void BeginPlay() override;

    // UPROPERTY 매크로: 언리얼 엔진에 변수를 노출시켜 에디터에서 수정하거나 가비지 컬렉터가 관리하게 함
    
    // 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator Components")
    class UBoxComponent* TriggerBox;

    // 설정 가능한 프로퍼티 (에디터 Details 패널에서 수정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings", Meta = (MakeEditWidget = true))
    FVector FirstTargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings", Meta = (MakeEditWidget = true))
    FVector SecondTargetLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings")
    float MaxSpeed = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings")
    float MinSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings")
    float Acceleration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator Settings")
    float HowToDelay = 20.f;
    
private:
    // 내부 상태 변수
    FVector StartLocation;
    float CurrentSpeed = 0.0f;

    UPROPERTY(VisibleAnywhere, Category="Elevator State")
    EElevatorState CurrentState = EElevatorState::IdleAtStart;

    FTimerHandle WaitTimerHandle;

    // 함수
    
    // UFUNCTION 매크로: 언리얼 엔진의 리플렉션 시스템에 함수를 등록
    // TriggerBox의 OnComponentBeginOverlap 이벤트에 동적으로 바인딩하기 위해 필수
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 15초 대기 후 두 번째 목적지로 이동을 시작하는 함수
    void MoveToSecondTarget();
};