// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "DroneMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDroneMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	UDroneMovementComponent();

protected:
	virtual void BeginPlay() override;

	//시뮬레이션 여부
	UPROPERTY(VisibleAnywhere)
	bool bSimulate;

	//중력
	UPROPERTY(EditAnywhere)
	float GravityForce;

	//중력 방향
	UPROPERTY(EditAnywhere)
	FVector GravityDirection;

	//추진력
	UPROPERTY(EditAnywhere)
	float ThrustForce;

	//추진 방향
	UPROPERTY(VisibleAnywhere)
	FVector ThrustDirection;

	//추력 편향 레벨
	UPROPERTY(EditAnywhere)
	float ThrustVectoringLevel;
	
	//사출 속도
	UPROPERTY(EditAnywhere)
	float LaunchSpeed;

	//탄성 계수
	UPROPERTY(EditAnywhere)
	float Bounciness;

	//공기 저항 계수
	UPROPERTY(EditAnywhere)
	float DragCoefficient;

	//질량
	UPROPERTY(EditAnywhere)
	float Mass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Launch(FVector LaunchDirection);

	UFUNCTION(BlueprintCallable)
	void Shock(FVector Direction, float Magnitude);

	UFUNCTION(BlueprintCallable)
	void ThrottleThrust(float Alpha);
	
	UFUNCTION(BlueprintCallable)
	void VectorThrust(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void Fall();
};
