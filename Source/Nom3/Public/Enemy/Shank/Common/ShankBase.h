// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Containers/Queue.h"
#include "Enemy/Core/EnemyBase.h"
#include "Interfaces/Damagable.h"
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

UENUM()
enum class EShankState : uint8
{
	Sleep = 0 UMETA(DisplayName = "Sleep"),
	FindPath = 1 UMETA(DisplayName = "FindPath"),
	FollowPath = 2 UMETA(DisplayName = "FollowPath"),
	ReverseThrust = 3 UMETA(DisplayName = "ReverseThrust"),
	Splash = 4 UMETA(DisplayName = "Splash"),
};

UCLASS()
class NOM3_API AShankBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	AShankBase();

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
	FVector TargetLocation;

	//이동 경로 위치를 담는 큐
	TQueue<FVector> PathQueue;

	//회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;

	//생크 스테이트 프로퍼티
	// __declspec(property(get=GetCurrentState, put=SetCurrentState)) EShankState SHANK_STATE;
	// FORCEINLINE EShankState GetCurrentState() const { return CurrentState; }
	// void SetCurrentState(const EShankState Value);

	//생크 스테이트 머신
	UShankStateMachineBase* GetCurrentStateMachine() const;
	void ChangeCurrentStateMachine(UShankStateMachineBase* StateMachineToChange);

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

	//일반적인 데미지 인터페이스 구현
	virtual void OnDamaged(FFireInfo Info) override;

	//크리티컬 데미지 인터페이스 구현
	virtual void OnCriticalDamaged(FFireInfo Info) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//현재 상태 열거형 프로퍼티
	UPROPERTY(VisibleAnywhere)
	EShankState CurrentState;

	//현재 상태 머신
	UPROPERTY()
	UShankStateMachineBase* CurrentStateMachine;

	//다이나믹 머터리얼 인스턴스
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DamageDynamicInstance;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> FireNiagara;

	UFUNCTION(BlueprintCallable)
	virtual void OnShotDown(const FVector ShotDir);
};
