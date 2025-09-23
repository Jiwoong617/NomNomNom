// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Dreg/NormalDreg.h"
#include "Enemy/Core/EnemyData.h"
#include "Core/DamageComponent.h"
#include "Enemy/Human/Common/HumanDamageComponent.h"
#include "Enemy/Human/Common/HumanStateMachineBase.h"
#include "Enemy/Human/Dreg/DregShooterComponent.h"
#include "Kismet/GameplayStatics.h"

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

#pragma region 춤추는 애니메이션 로드

	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance1.AS_Dance1"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance4.AS_Dance4"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance5.AS_Dance5"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance6.AS_Dance6"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance7.AS_Dance7"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance8.AS_Dance8"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance9.AS_Dance9"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance11.AS_Dance11"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}
	
	//대기 애니메이션 로드
	if (static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		Finder(TEXT("/Game/Asset/Dreg/Model/AS_Dance14.AS_Dance14"));
		Finder.Succeeded())
	{
		WaitAnimationAssets.Add(Finder.Object);
	}

#pragma endregion 
	
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
	DamageComp->SetBoxExtent(FVector(70, 70, 70));
	DamageComp->SetRelativeLocation(FVector(0, 0, 100));
	DamageComp->SetupAttachment(GetMesh());

	//크리티컬 데미지 컴포넌트 부착
	CriticalDamageComp = CreateDefaultSubobject<UHumanDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetBoxExtent(FVector(36, 36, 36));
	CriticalDamageComp->SetRelativeLocation(FVector(0, 0, 200));
	CriticalDamageComp->SetupAttachment(GetMesh());

	//총 스켈레탈 메시 컴포넌트 부착
	GunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("GumMeshComp"));
	GunMeshComp->SetCollisionProfileName(FName("NoCollision"));
	
	//스켈레탈 메시 로드
	if (static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Finder(TEXT("/Game/Asset/Dreg/GUN.GUN"));
		Finder.Succeeded())
	{
		GunMeshComp->SetStaticMesh(Finder.Object);
		GunMeshComp->SetRelativeLocation(FVector(0, 0, 0));
		GunMeshComp->SetupAttachment(GetMesh(), FName("GunSocket"));
	}
	
	//드렉 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UDregShooterComponent>(FName("ShooterComp"));
	ShooterComp->SetupAttachment(GunMeshComp, FName("GunFireSocket"));

	//드렉 사망 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase>
		Finder(TEXT("/Game/Asset/Dreg/Sound/SC_Death.SC_Death"));
		Finder.Succeeded())
	{
		DieSound = Finder.Object;
	}
}

void ANormalDreg::BeginPlay()
{
	Super::BeginPlay();

	//랜덤 대기 애니메이션 재생                                                                                                             
	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	GetMesh()->PlayAnimation(WaitAnimationAssets[FMath::RandRange(0, WaitAnimationAssets.Num() - 1)], true);
	
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

	//사망 사운드 재생
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DieSound, GetActorLocation());
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