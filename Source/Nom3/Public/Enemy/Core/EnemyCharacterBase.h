// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy/Interfaces/OnAimByPlayerSight.h"
#include "GameFramework/Character.h"
#include "Interfaces/CriticalDamagable.h"
#include "Interfaces/Damagable.h"
#include "Interfaces/NoticePawn.h"
#include "EnemyCharacterBase.generated.h"

//전방 선언
class UEnemyData;
class UEnemyStatus;
class AAIController;
class UWidgetComponent;
class UStateMachineBase;
class UNavigationSystemV1;
class UEnemyHealthComponent;
class UDamageActorPoolWorldSubsystem;

UCLASS()
class NOM3_API AEnemyCharacterBase : public ACharacter, public IOnAimByPlayerSight, public IDamagable, public ICriticalDamagable, public INoticePawn
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//에너미 데이터
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEnemyData> EnemyData;

	//AI 컨트롤러
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> AIController;

	//네비게이션 시스템
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNavigationSystemV1> NavigationSystem;
	
	//현재 목표로 하는 폰
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetPawn;

	//체력 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEnemyHealthComponent> HealthComp;

	//체력 등의 스테이터스를 표시하는 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> StatusWidgetComp;

	//실제로 사용 중인 스테이터스 위젯
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEnemyStatus> StatusWidget;

	//스테이트 머신
	UStateMachineBase* GetCurrentStateMachine() const;
	void ChangeCurrentStateMachine(UStateMachineBase* StateMachineToChange);

	//피조준 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnAimByPlayerSight() override;

	//일반적인 데미지 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnDamaged(FFireInfo Info) override;

	//크리티컬 데미지 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnCriticalDamaged(FFireInfo Info) override;

	//폰 인식 인터페이스
	UFUNCTION(BlueprintCallable)
	virtual void OnNoticePawn(AActor* DetectedPawn) override;

	UFUNCTION()
	virtual void OnDie();

	//플레이어를 바라보는 방향
	UFUNCTION()
	FVector GetPlayerGazeDir() const;

	//플레이어를 바라보는 방향과 수직이고 플레이어 상단 방향
	UFUNCTION()
	FVector GetPlayerGazeUpDir() const;

	//플레이어를 바라보는 방향과 수직이고 플레이어 우측 방향
	UFUNCTION()
	FVector GetPlayerGazeRightDir() const;

protected:
	//현재 상태 머신
	UPROPERTY()
	UStateMachineBase* CurrentStateMachine;
	
	//데미지 액터 풀링 게임 인스턴스 서브시스템 참조
	UPROPERTY()
	TObjectPtr<UDamageActorPoolWorldSubsystem> DamageActorPool;

	//AI 컨트롤러 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAIController> DefaultAIControllerClass;

	//피조준 타이머 핸들
	UPROPERTY()
	FTimerHandle OnSightTimerHandle;

	//대기 애니메이션 에셋 배열
	UPROPERTY(VisibleAnywhere)
	TArray<UAnimationAsset*> WaitAnimationAssets;

	//애니메이션 블루프린트 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBlueprintClass;

	//사망 사운드
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DieSound;
};
