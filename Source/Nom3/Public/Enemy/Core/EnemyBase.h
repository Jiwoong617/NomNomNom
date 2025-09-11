// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

//델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEventSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnArmorChangedEventSignature, float);
DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature);

//전방 선언
class UDamageActorPoolGameInstanceSubsystem;

UCLASS(Abstract)
class NOM3_API AEnemyBase : public AActor
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

	//아머 관련 프로퍼티와 델리게이트
	__declspec(property(get=GetArmor, put=SetArmor)) float ARMOR;
	FORCEINLINE float GetArmor() const { return Armor; };
	FORCEINLINE void SetArmor(const float Value)
	{
		this->Armor = Value;
		OnArmorChanged.Broadcast(Armor);
	};
	FOnArmorChangedEventSignature OnArmorChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	int32 Health;

	UPROPERTY(VisibleAnywhere)
	float Armor;

	UPROPERTY()
	TObjectPtr<UDamageActorPoolGameInstanceSubsystem> DamageActorPool;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetPawn;
};
