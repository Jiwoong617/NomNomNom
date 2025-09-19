// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemyCharacterBase.h"
#include "Dreg.generated.h"

//전방 선언
class UDamageComponent;

UCLASS()
class NOM3_API ADreg : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	ADreg();

	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> DamageComp;

	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> CriticalDamageComp;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
