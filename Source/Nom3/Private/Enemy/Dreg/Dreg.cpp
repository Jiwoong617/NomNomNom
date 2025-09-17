// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Dreg/Dreg.h"

#include "Enemy/Dreg/DregDamageComponent.h"

ADreg::ADreg()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//일반적인 데미지 컴포넌트 부착
	DamageComp = CreateDefaultSubobject<UDregDamageComponent>(FName("DamageComp"));
	DamageComp->SetupAttachment(GetMesh());

	//크리티컬 데미지 컴포넌트 부착
	CriticalDamageComp = CreateDefaultSubobject<UDregDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetupAttachment(GetMesh());
}

void ADreg::BeginPlay()
{
	Super::BeginPlay();

	//체력이 다했을 경우
	OnDeath.AddUFunction(this, FName("OnShotDown"));
	
	//1초 뒤에 경로 ?? 상태로 전환
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		
	}, 1, false);
}

void ADreg::OnAimByPlayerSight()
{
	Super::OnAimByPlayerSight();
}

void ADreg::OnDamaged(FFireInfo Info)
{
	Super::OnDamaged(Info);
}

void ADreg::OnCriticalDamaged(FFireInfo Info)
{
	Super::OnCriticalDamaged(Info);
}

// Called every frame
void ADreg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADreg::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}