// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Containers/Queue.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "ShankBase.generated.h"

//전방 선언
class UShankDamageComponent;
class UNiagaraSystem;
class UDamageComponent;
class UArrowComponent;
class AScoutShankProjectile;
class UShankReverseThrustStateMachine;
class UScoutShankFindPathStateMachine;
class UShankStateMachineBase;
class UDroneMovementComponent;
class USphereComponent;
class UShankFollowPathStateMachine;

UENUM()
enum class EShankType : uint8
{
	ScoutShank = 0 UMETA(DisplayName = "ScoutShank"),
	SelfDestructShank = 1 UMETA(DisplayName = "SelfDestructShank"),
};

UCLASS()
class NOM3_API AShankBase : public AEnemyActorBase
{
	GENERATED_BODY()

public:
	AShankBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	//씬 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MeshSceneComp;

	//스켈레탈 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	//데미지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankDamageComponent> DamageComp;

	//드론 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneMovementComponent> DroneMoveComp;

	//경로 탐색 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankStateMachineBase> FindPathStateMachine;

	//경로 추적 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankStateMachineBase> FollowPathStateMachine;

	//역추진 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankStateMachineBase> ReverseThrustStateMachine;

	//현재 목표 위치
	UPROPERTY(VisibleAnywhere)
	FVector Destination;

	//이동 경로 위치를 담는 큐
	TQueue<FVector> PathQueue;

	//회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

protected:
	//다이나믹 머터리얼 인스턴스
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DamageDynamicInstance;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> ExplosionNiagara;

	UFUNCTION(BlueprintCallable)
	virtual void OnShotDown(const FVector ShotDir);
};
