// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActorBase.h"
#include "Components/ActorComponent.h"
#include "EnemyHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UEnemyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyHealthComponent();

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

	//초기화
	void Init(const int32 HP);

protected:
	//체력 프로퍼티
	UPROPERTY(VisibleAnywhere)
	int32 Health;
};
