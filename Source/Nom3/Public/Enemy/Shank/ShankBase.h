// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankStateMachineBase.h"
#include "Enemy/Core/EnemyBase.h"
#include "Enemy/Interfaces/OnAimByPlayerSight.h"
#include "ShankBase.generated.h"

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> NormalHitComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CriticalHitComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDroneMovementComponent> DroneMoveComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankFindPathStateMachine> FindPathStateMachine;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankFollowPathStateMachine> CircleStateMachine;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankReverseThrustStateMachine> ReverseThrustStateMachine;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ThrottleControlCurve;

	UPROPERTY(EditAnywhere)
	float KeepDistance;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle DodgeTimerHandle;

	//플레이어 시선 노출 인터페이스 구현
	UFUNCTION(BlueprintCallable)
	virtual void OnAimByPlayerSight() override;

	//생크 스테이트 프로퍼티
	__declspec(property(get=GetCurrentState, put=SetCurrentState)) EShankState SHANK_STATE;
	FORCEINLINE EShankState GetCurrentState() const { return CurrentState; }
	void SetCurrentState(const EShankState Value);
	
private:
	UPROPERTY(VisibleAnywhere)
	EShankState CurrentState;

	UPROPERTY()
	UShankStateMachineBase* CurrentStateMachine;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnShotByPlayer(FVector ShotDir, int Attack);

	UFUNCTION(BlueprintCallable)
	void OnShotDown(const FVector ShotDir);
};
