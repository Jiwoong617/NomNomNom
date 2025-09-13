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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamagedDelegate, FFireInfo, info);

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

	EBodyType BodyType = EBodyType::None;
	
	virtual void OnHitBody(FFireInfo& Info);
	
	virtual void OnHitHead(FFireInfo& Info);

	FOnDamagedDelegate OnDamagedDelegate;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//충돌 델리게이트 함수
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//박스 범위를 조절하는 초기화 메서드
	virtual void Init(FVector BoxSize, ECollisionChannel Channel,
		FName CollisionPresetName, EBodyType BodyType);

	//박스 범위를 조절하지 않는 초기화 메서드
	virtual void Init(ECollisionChannel Channel,
	FName CollisionPresetName, EBodyType BodyType);

	virtual void OnDamaged(FFireInfo Info);
	
	//소유자 사망 시 비활성화하기 위한 메서드
	UFUNCTION()
	void Inactive();
};