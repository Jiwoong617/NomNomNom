// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoLight.generated.h"

UCLASS()
class NOM3_API AAutoLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAutoLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "AutoLight")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "AutoLight", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "AutoLight" , meta = (AllowPrivateAccess = "true"))
	TArray<class ALight*> LightsActor;
	
	UPROPERTY(EditAnywhere, Category = "AutoLight", meta = (AllowPrivateAccess = "true"))
	float DelayBetweenLight = 1.5f;

	void ActivateLight();

	int32 CurrentLightIndex = 0;

	FTimerHandle LightTimer;

	bool bHasBeenTriggered = false;

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
