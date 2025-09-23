// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Core/EnemyCharacterBase.h"
#include "HumanBase.generated.h"

class UNiagaraSystem;
//전방 선언
class UHumanStateMachineBase;
class UNavigationSystemV1;
class UDamageComponent;

UCLASS()
class NOM3_API AHumanBase : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AHumanBase();
	
	//씬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> MeshSceneComp;

	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> DamageComp;

	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDamageComponent> CriticalDamageComp;

	//대기 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHumanStateMachineBase> IdleStateMachine;

	//이동 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHumanStateMachineBase> MoveStateMachine;

	//회피 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHumanStateMachineBase> EvadeStateMachine;

	virtual void OnNoticePawn(AActor* DetectedPawn) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnDie() override;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> DieNiagara;

public:
	virtual void Tick(float DeltaTime) override;

	bool FindReachableLocation(FVector& TargetLocation) const;
};