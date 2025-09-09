// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingObject.generated.h"

UCLASS()
class NOM3_API AMovingObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

<<<<<<< HEAD
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ObjectMesh;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Object | Movement")
	float MoveSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Object | Movement", Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving Object | State") EMovementState MovementState;
=======
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingObject")
	class UStaticMeshComponent* ObjectMesh;

	//속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingObject")
	float ObjectSpeed;

	//목표 지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingObject", Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	//움직임 트리거
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingObject")
	bool bIsTriggered; 
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)

	//게임 시작 시의 월드 위치
	FVector StartLocation;
	//목표 지점의 월드 위치
	FVector GlobalTargetLocation;

private:
	
};