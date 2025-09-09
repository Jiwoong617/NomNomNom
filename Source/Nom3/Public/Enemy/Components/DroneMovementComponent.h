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
	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bSimulate;

	//중력
	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityForce;

	//중력 방향
	UPROPERTY(EditAnywhere, Category="Movement")
	FVector GravityDir;

	//순항 추진력
	UPROPERTY(EditAnywhere, Category="Movement")
	float CruiseThrustForce;

	//회피 추진 레벨
	UPROPERTY(EditAnywhere, Category="Movement")
	float EvadeThrustLevel;
	
	//현재 추진력
	UPROPERTY(VisibleAnywhere, Category="Movement")
	float ThrustForce;

	//현재 추진 방향
	UPROPERTY(VisibleAnywhere, Category="Movement")
	FVector ThrustDir;

	//추력 편향 레벨
	UPROPERTY(EditAnywhere, Category="Movement")
	float ThrustVectoringLevel;
	
	//사출 속도
	UPROPERTY(EditAnywhere, Category="Movement")
	float LaunchSpeed;

	//파괴 충격
	UPROPERTY(EditAnywhere, Category="Movement")
	float SplashSpeed;

	//탄성 계수
	UPROPERTY(EditAnywhere, Category="Movement")
	float Bounciness;

	//공기 저항 계수
	UPROPERTY(EditAnywhere, Category="Movement")
	float DragCoefficient;

	//질량
	UPROPERTY(EditAnywhere, Category="Movement")
	float Mass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Launch(FVector LaunchDir);

	UFUNCTION(BlueprintCallable)
	void Splash(FVector SplashDir);

	UFUNCTION(BlueprintCallable)
	void ThrottleThrustByLevel(float Level);

	UFUNCTION(BlueprintCallable)
	void ThrottleToCruiseThrust();

	UFUNCTION(BlueprintCallable)
	void ThrottleHighToEvade();

	UFUNCTION(BlueprintCallable)
	void ThrottleOff();
	
	UFUNCTION(BlueprintCallable)
	void ReverseVectorThrust(float Accel);
	
	UFUNCTION(BlueprintCallable)
	void VectorThrust(FVector VectorDir);

	UFUNCTION(BlueprintCallable)
	void Fall();
};
