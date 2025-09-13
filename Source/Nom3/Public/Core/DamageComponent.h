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
	Head = 2		UMETA(DisplayName = "Head")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UDamageComponent : public UBoxComponent
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	EBodyType BodyType = EBodyType::None;
	
	virtual void OnHitBody(FFireInfo& info);
	
	virtual void OnHitHead(FFireInfo& info);
	
public:
	//충돌 델리게이트 함수
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//박스 범위를 조절하는 초기화 메서드
	virtual void Init(FVector boxSize, ECollisionChannel channel,
		FName collisionPresetName, EBodyType bodyType);

	//박스 범위를 조절하지 않는 초기화 메서드
	virtual void Init(ECollisionChannel channel,
	FName collisionPresetName, EBodyType bodyType);

	//소유자 사망 시 비활성화하기 위한 메서드
	UFUNCTION()
	void Inactive();
};