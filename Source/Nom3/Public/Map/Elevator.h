// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovingObject.h"
#include "Elevator.generated.h"

UCLASS()
class NOM3_API AElevator : public AMovingObject
{
	GENERATED_BODY()

public:
	void OnInteract(TObjectPtr<class UInputComponent> Object);
	AElevator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void OnInteract(AActor OtherActor);

	// 플레이어 감지를 위한 트리거 볼륨
	UPROPERTY(VisibleAnywhere, Category = "Elevator")
	class UBoxComponent* TriggerBox;

	// 가감속 구간에서의 최저 속도
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float MinSpeed;

	// 최대 속도까지 도달하는 가속도 (클수록 더 빨리 최대 속도에 도달)
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float Acceleration;
    
	// 원래 설정된 최대 속도를 저장할 변수
	float MaxSpeed;

	// // 플레이어가 올라탔을 때 호출될 함수
	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	// // 플레이어가 내렸을 때 호출될 함수
	// UFUNCTION()
	// void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};