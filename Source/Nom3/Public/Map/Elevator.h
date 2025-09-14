#pragma once

#include "CoreMinimal.h"
#include "MovingObject.h"
#include "Elevator.generated.h"

UCLASS()
class NOM3_API AElevator : public AMovingObject
{
	GENERATED_BODY()

public:
	AElevator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 플레이어 감지를 위한 트리거 볼륨
	UPROPERTY(VisibleAnywhere, Category = "Elevator")
	class UBoxComponent* TriggerBox;

	// 최소 속도 
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float MinSpeed = 50.0f;

	// 최대 속도
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float MaxSpeed = 200.0f;

	// 가속도 
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float Acceleration = 2.0f;

	// 현재 속도
	float CurrentSpeed = 0.0f;

	// 트리거 여부
	bool bIsTriggered = false;

	// 충돌 처리
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};