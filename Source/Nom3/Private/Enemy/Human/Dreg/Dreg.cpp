// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Dreg/Dreg.h"

#include "Core/DamageComponent.h"
#include "Enemy/Human/Common/HumanStateMachineBase.h"
#include "Enemy/Human/Dreg/DregShooterComponent.h"

ADreg::ADreg()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 로드
	
	//드렉 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UDregShooterComponent>(FName("ShooterComp"));
	ShooterComp->SetupAttachment(GetMesh());
}

void ADreg::BeginPlay()
{
	Super::BeginPlay();
	
	//자동 사격
	ShooterComp->ActiveAutoFire();

	//일반적인 데미지 컴포넌트 초기화
	DamageComp->Init(ECC_EngineTraceChannel2, FName("Body"), EBodyType::Body);

	//일반적인 데미지 컴포넌트 초기화
	CriticalDamageComp->Init(ECC_EngineTraceChannel1, FName("Head"), EBodyType::Head);
}

void ADreg::OnAimByPlayerSight()
{
	//회피가 불가능한 상태가 아니라면
	if (CurrentStateMachine == nullptr)
	{
		return;
	}
	
	//회피 후 시간이 충분히 지나지 않았다면
	if (EvadeTimerHandle.IsValid())
	{
		return;
	}

	//회피 상태로 전환
	ChangeCurrentStateMachine(EvadeStateMachine);
}