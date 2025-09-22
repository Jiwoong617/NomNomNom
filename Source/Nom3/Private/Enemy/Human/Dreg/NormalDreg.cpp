// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Dreg/NormalDreg.h"
#include "Enemy/Core/EnemyData.h"
#include "Core/DamageComponent.h"
#include "Enemy/Human/Common/HumanDamageComponent.h"
#include "Enemy/Human/Common/HumanStateMachineBase.h"
#include "Enemy/Human/Dreg/DregShooterComponent.h"

ANormalDreg::ANormalDreg()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//데이터 로드
	if (static ConstructorHelpers::FObjectFinder<UEnemyData>
		Finder(TEXT("/Game/Data/DA_Dreg.DA_Dreg"));
		Finder.Succeeded())
	{
		EnemyData = Finder.Object;
	}
	
	//스켈레탈 메시 로드
	if (static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		Finder(TEXT("/Game/Asset/Dreg/Model/SKM_Dreg.SKM_Dreg"));
		Finder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Finder.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetupAttachment(MeshSceneComp);
	}

	//일반적인 데미지 컴포넌트 부착
	DamageComp = CreateDefaultSubobject<UHumanDamageComponent>(FName("DamageComp"));
	DamageComp->SetBoxExtent(FVector(50, 50, 70));
	DamageComp->SetRelativeLocation(FVector(0, 0, 100));
	DamageComp->SetupAttachment(GetMesh());

	//크리티컬 데미지 컴포넌트 부착
	CriticalDamageComp = CreateDefaultSubobject<UHumanDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetBoxExtent(FVector(36, 36, 36));
	CriticalDamageComp->SetRelativeLocation(FVector(0, 0, 200));
	CriticalDamageComp->SetupAttachment(GetMesh());
	
	//드렉 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UDregShooterComponent>(FName("ShooterComp"));
	ShooterComp->SetupAttachment(GetMesh());
}

void ANormalDreg::BeginPlay()
{
	Super::BeginPlay();
	
	//자동 사격
	ShooterComp->ActiveAutoFire();

	//일반적인 데미지 컴포넌트 초기화
	CriticalDamageComp->Init(ECC_EngineTraceChannel1, FName("Head"), EBodyType::Head);
	
	//일반적인 데미지 컴포넌트 초기화
	DamageComp->Init(ECC_EngineTraceChannel2, FName("Body"), EBodyType::Body);
}

void ANormalDreg::OnDie()
{
	Super::OnDie();

	//사격 컴포넌트 비활성화
	ShooterComp->InactiveAutoFire();
}

void ANormalDreg::OnAimByPlayerSight()
{
	Super::OnAimByPlayerSight();
	
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

	//무한 회피를 방지하는 타이머 설정
	GetWorldTimerManager().SetTimer(EvadeTimerHandle, [this]()
	{
		//회피 타이머 핸들 초기화
		GetWorldTimerManager().ClearTimer(EvadeTimerHandle);
	}, 10, false);
}
