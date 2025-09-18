// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "DroneBase.generated.h"

//전방 선언
class UDroneDamageComponent;
class UNiagaraSystem;
class UDamageComponent;
class UArrowComponent;
class AScoutShankProjectile;
class UDroneReverseThrustStateMachine;
class UScoutShankFindPathStateMachine;
class UDroneStateMachineBase;
class UDroneMovementComponent;
class USphereComponent;
class UDroneFollowPathStateMachine;

UCLASS()
class NOM3_API ADroneBase : public AEnemyActorBase
{
	GENERATED_BODY()

public:
	ADroneBase();

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

	//드론 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneMovementComponent> DroneMoveComp;

	//경로 탐색 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneStateMachineBase> FindPathStateMachine;

	//경로 추적 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneStateMachineBase> FollowPathStateMachine;

	//역추진 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneStateMachineBase> ReverseThrustStateMachine;

	//현재 목표 위치
	UPROPERTY(VisibleAnywhere)
	FVector Destination;

	//이동 경로 위치를 담는 큐
	TQueue<FVector> PathQueue;

protected:

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> ExplosionNiagara;

	UFUNCTION(BlueprintCallable)
	virtual void OnShotDown(const FVector ShotDir);
};
