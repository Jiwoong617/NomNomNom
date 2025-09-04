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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MovingObject")
	class UStaticMeshComponent* ObjectMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingObject")
	float ObjectSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingObject")
	bool bPointture;

//	UFUNCTION()
//	void MovingObject(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & HitResult);
};
