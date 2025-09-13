// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Damagable.h"
#include "DamageComponent.generated.h"

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Body = 1		UMETA(DisplayName = "Body"),
	Head = 1		UMETA(DisplayName = "Head")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDamageComponent : public UBoxComponent, public IDamagable
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	EBodyType BodyType = EBodyType::None;
	
protected:
	virtual void OnHitBody(FFireInfo& info);
	virtual void OnHitHead(FFireInfo& info);
	
public:
	virtual void OnDamaged(FFireInfo FireInfo) override;
	
	virtual void Init(FVector boxSize, AActor* owner, ECollisionChannel channel,
		FName collisionPresetName, EBodyType bodyType);
};
