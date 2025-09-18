// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Dreg/Dreg.h"

#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Dreg/DregDamageComponent.h"

ADreg::ADreg()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//일반적인 데미지 컴포넌트 부착
	DamageComp = CreateDefaultSubobject<UDregDamageComponent>(FName("DamageComp"));
	DamageComp->SetupAttachment(GetMesh());

	//크리티컬 데미지 컴포넌트 부착
	CriticalDamageComp = CreateDefaultSubobject<UDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetupAttachment(GetMesh());
}

void ADreg::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADreg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}
}

void ADreg::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

