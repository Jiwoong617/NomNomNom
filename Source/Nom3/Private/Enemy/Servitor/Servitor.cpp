// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Servitor/Servitor.h"



#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Servitor/ServitorPathFindStateMachine.h"
#include "Enemy/Servitor/ServitorShooterComponent.h"
#include "Enemy/Shank/Common/DroneDamageComponent.h"

AServitor::AServitor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 로드
	if (static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(
		TEXT("/Game/Asset/Boss_Servitor/SKM_Servitor.SKM_Servitor"));
		Finder.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(Finder.Object);
		SkeletalMeshComp->SetRelativeLocation(FVector(0, 0, -110));
		SkeletalMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	}

	//일반적인 데미지 컴포넌트
	DamageComp = CreateDefaultSubobject<UDroneDamageComponent>(FName("DamageComp"));
	DamageComp->SetRelativeLocation(FVector(0, 0, 140));
	DamageComp->SetBoxExtent(FVector(100, 100, 100));
	DamageComp->SetupAttachment(SkeletalMeshComp);

	//크리티컬 데미지 컴포넌트
	CriticalDamageComp = CreateDefaultSubobject<UDroneDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetRelativeLocation(FVector(0, 110, 140));
	CriticalDamageComp->SetBoxExtent(FVector(50, 25, 50));
	CriticalDamageComp->SetupAttachment(SkeletalMeshComp);

	//서비터 전용의 경로 탐색 스테이트 머신 부착
	FindPathStateMachine = CreateDefaultSubobject<UServitorPathFindStateMachine>(FName("FindPathStateMachine"));

	//서비터 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UServitorShooterComponent>(FName("ShooterComp"));
	ShooterComp->SetupAttachment(SkeletalMeshComp);
	ShooterComp->SetRelativeLocation(FVector(0, 250, 140));
}

void AServitor::BeginPlay()
{
	Super::BeginPlay();

	//초기화
	HealthComp->Init(75000);

	//초기화
	CriticalDamageComp->Init(ECC_GameTraceChannel1, FName("Head"), EBodyType::Head);

	//초기화
	DamageComp->Init(ECC_GameTraceChannel2, FName("Body"), EBodyType::Body);

	//자동 사격
	ShooterComp->ActiveAutoFire();
}

void AServitor::OnShotDown(const FVector ShotDir)
{
	Super::OnShotDown(ShotDir);

	//사격 비활성화
	ShooterComp->InactiveAutoFire();
}

// 여기서 죽었나 안죽었나를 알아볼수 있나?
// ㄴ 상속되어있는 곳에서 확인해야함

