// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponent.h"
#include "PlayerDamageComponent.generated.h"


class ANomPlayer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UPlayerDamageComponent : public UDamageComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	ANomPlayer* Owner;
	
protected:
	virtual void OnHitBody(FFireInfo& info);
	virtual void OnHitHead(FFireInfo& info);
	
public:
	virtual void Init(FVector boxSize, AActor* owner, ECollisionChannel channel, FName collisionPresetName, EBodyType bodyType) override;
};
