// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Interfaces/OnAimByPlayerSight.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CriticalDamagable.h"
#include "Interfaces/Damagable.h"
#include "EnemyBase.generated.h"

//델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEventSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnArmorChangedEventSignature, float);
DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature);

//전방 선언
class UDamageActorPoolWorldSubsystem;

UCLASS(Abstract)
class NOM3_API AEnemyBase : public AActor, public IOnAimByPlayerSight, public IDamagable, public ICriticalDamagable
{
	GENERATED_BODY()

public:
	AEnemyBase();

	//체력 관련 프로퍼티와 델리게이트
	__declspec(property(get=GetHP, put=SetHP)) int32 HP;
	FORCEINLINE int32 GetHP() const { return Health; };
	FORCEINLINE void SetHP(const int32 Value)
	{
		this->Health = Value;

		if (Health <= 0)
		{
			OnDeath.Broadcast();
		}
		else
		{
			OnHPChanged.Broadcast(Health);	
		}
	};
	FOnHealthChangedEventSignature OnHPChanged;
	FOnDeathEventSignature OnDeath;

	//피조준 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnAimByPlayerSight() override;

	//일반적인 데미지 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnDamaged(FFireInfo Info) override;

	//크리티컬 데미지 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnCriticalDamaged(FFireInfo Info) override;

protected:
	virtual void BeginPlay() override;

	//체력 프로퍼티
	UPROPERTY(VisibleAnywhere)
	int32 Health;

	//데미지 액터 풀링 게임 인스턴스 서브시스템 참조
	UPROPERTY()
	TObjectPtr<UDamageActorPoolWorldSubsystem> DamageActorPool;

public:

	//현재 목표로 하는 폰
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetPawn;
};
