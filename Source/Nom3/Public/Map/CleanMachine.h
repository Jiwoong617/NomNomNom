// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Nom3/Public/Interfaces/Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CleanMachine.generated.h"

UCLASS()
class NOM3_API ACleanMachine : public AActor, public IInteractable
{
	GENERATED_BODY()
	enum class ECleanMState : uint8
	{
		FollowingSpline,
		TargetLockOn,
		Chasing
	};

public:
	// Sets default values for this actor's properties
	ACleanMachine();
	// 액터에 필요한 매쉬
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
	UStaticMeshComponent* CleanMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
	class UBoxComponent* HitPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	AActor* TargetSplineActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	class USplineComponent* PathSpline;


	// 이동 관련 함수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
	float speed = 500.f;
	float Maxspeed = 5000.f;
	float SuspensionForce = 5000.f;
	float SuspensionHeight = 10.f;
	float lookDistance = 1000.f;

	//스플라인 맥터 지정용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
	AActor* SplineComp;

	virtual void OnInteract(AActor OtherActor) override;
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 타이머 함수
	FTimerManager DelayTimer;
	// 상호작용 함수
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void TraceMove(float DeltaTime);
	void FollwSpline(float DeltaTime);
	float DistanceAlongSpline;
private:
	UPROPERTY()
	class USplineComponent* TargetSpline;
	ECleanMState normalstate = ECleanMState::FollowingSpline;
	
};
