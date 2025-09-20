// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanBase.h"
#include "NavigationSystem.h"
#include "Core/DamageComponent.h"
#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Human/Common/HumanEvadeStateMachine.h"
#include "Enemy/Human/Common/HumanIdleStateMachine.h"
#include "Enemy/Human/Common/HumanMoveStateMachine.h"

AHumanBase::AHumanBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	//일반적인 데미지 컴포넌트 부착
	DamageComp = CreateDefaultSubobject<UDamageComponent>(FName("DamageComp"));
	DamageComp->SetupAttachment(GetMesh());

	//크리티컬 데미지 컴포넌트 부착
	CriticalDamageComp = CreateDefaultSubobject<UDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetupAttachment(GetMesh());

	//대기 상태 머신
	IdleStateMachine = CreateDefaultSubobject<UHumanIdleStateMachine>(FName("IdleStateMachine"));

	//이동 상태 머신
	MoveStateMachine = CreateDefaultSubobject<UHumanMoveStateMachine>(FName("MoveStateMachine"));

	//회피 상태 머신
	EvadeStateMachine = CreateDefaultSubobject<UHumanEvadeStateMachine>(FName("EvadeStateMachine"));
}

void AHumanBase::BeginPlay()
{
	Super::BeginPlay();

	//2초 후에 대기 상태로 전환
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		ChangeCurrentStateMachine(IdleStateMachine);
	}, 2, false);
}

void AHumanBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}
}

void AHumanBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AHumanBase::FindReachableLocation(FVector& TargetLocation) const
{
	//실패할 경우 1m 범위 내 도달 가능한 지점을 랜덤으로 획득
	FNavLocation ReachableLocation;
	for (int32 i = 1; i <= 10; i++)
	{
		//검색 영역을 늘려가면서 성공할 때까지 반복
		if (NavigationSystem->ProjectPointToNavigation(TargetLocation, ReachableLocation, FVector(5 * i)))
		{
			TargetLocation = ReachableLocation.Location;
			return true;
		}
	}

	return false;
}