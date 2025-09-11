// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Containers/Queue.h"
#include "Enemy/Core/EnemyBase.h"
#include "Enemy/Interfaces/OnAimByPlayerSight.h"
#include "ShankBase.generated.h"

class UArrowComponent;
class AScoutShankBullet;
class UShankReverseThrustStateMachine;
class UShankFindPathStateMachine;
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
class NOM3_API AShankBase : public AEnemyBase, public IOnAimByPlayerSight
{
	GENERATED_BODY()

public:
	AShankBase();

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	//스켈레탈 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	//일반 공격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> NormalHitComp;

	//치명타 공격 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CriticalHitComp;

	//드론 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneMovementComponent> DroneMoveComp;

	//경로 탐색 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankFindPathStateMachine> FindPathStateMachine;

	//경로 추적 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankFollowPathStateMachine> CircleStateMachine;

	//역추진 상태 머신
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankReverseThrustStateMachine> ReverseThrustStateMachine;

	//현재 목표 위치
	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	//이동 경로 위치를 담는 큐
	TQueue<FVector> PathQueue;

	//회피 타이머 핸들
	UPROPERTY(VisibleAnywhere)
	FTimerHandle EvadeTimerHandle;

	//생크 스테이트 프로퍼티
	__declspec(property(get=GetCurrentState, put=SetCurrentState)) EShankState SHANK_STATE;
	FORCEINLINE EShankState GetCurrentState() const { return CurrentState; }
	void SetCurrentState(const EShankState Value);

	//플레이어 시선 노출 인터페이스 구현
	UFUNCTION(BlueprintCallable)
	virtual void OnAimByPlayerSight() override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//현재 상태 열거형 프로퍼티
	UPROPERTY(VisibleAnywhere)
	EShankState CurrentState;

	//현재 상태 머신
	UPROPERTY()
	UShankStateMachineBase* CurrentStateMachine;
	
	UFUNCTION(BlueprintCallable)
	void OnShotByPlayer(FVector ShotDir, int Attack);

	UFUNCTION(BlueprintCallable)
	void OnShotDown(const FVector ShotDir);
};
