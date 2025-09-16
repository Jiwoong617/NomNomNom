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

	//상속받은 자식 클래스에서 재할당 필요함
	EBodyType BodyType = EBodyType::None;

	//몸에 맞았을 때 처리하는 메서드로 캐릭터, 에너미에 따라서 오버라이드
	virtual void OnHitBody(const FFireInfo& Info);

	//머리에 맞았을 때 처리하는 메서드로 캐릭터, 에너미에 따라서 오버라이드
	virtual void OnHitHead(const FFireInfo& Info);

public:
	//충돌 델리게이트 함수
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//박스 범위를 조절하는 초기화 메서드
	virtual void Init(FVector BoxSize, ECollisionChannel Channel,
		FName CollisionPresetName, EBodyType BodyType);

	//박스 범위를 조절하지 않는 초기화 메서드
	virtual void Init(ECollisionChannel Channel,
	FName CollisionPresetName, EBodyType BodyType);

	//데미지를 주기 위해서 외부에서 호출하는 메서드
	void OnDamaged(const FFireInfo& Info);
	
	//소유자 사망 시 비활성화하기 위한 메서드
	UFUNCTION()
	void Inactive();
};