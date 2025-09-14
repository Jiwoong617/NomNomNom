// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Enemy/Shank/ShankFollowPathStateMachine.h"
#include "Components/SphereComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Damage/DamageActorPoolGameInstanceSubsystem.h"
#include "Enemy/Shank/ShankFindPathStateMachine.h"
#include "Enemy/Shank/ShankReverseThrustStateMachine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nom3/Nom3.h"

AShankBase::AShankBase() :
	CurrentState(EShankState::Sleep)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 컴포넌트
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	SetRootComponent(SphereComp);

	//메시 씬 컴포넌트
	MeshSceneComp = CreateDefaultSubobject<USceneComponent>(FName("MeshSceneComp"));
	MeshSceneComp->SetupAttachment(RootComponent);

	//스켈레탈 컴포넌트
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComp"));
	SkeletalMeshComp->SetCollisionProfileName(FName("NoCollision"));
	SkeletalMeshComp->SetupAttachment(MeshSceneComp);

	//드론 무브먼트 컴포넌트
	DroneMoveComp = CreateDefaultSubobject<UDroneMovementComponent>(FName("DroneMoveComp"));

	//경로 탐색 상태 머신
	FindPathStateMachine = CreateDefaultSubobject<UShankFindPathStateMachine>(FName("FindPathStateMachine"));
	
	//경로 추적 상태 머신
	CircleStateMachine = CreateDefaultSubobject<UShankFollowPathStateMachine>(FName("CircleStateMachine"));

	//역추진 상태 머신
	ReverseThrustStateMachine = CreateDefaultSubobject<UShankReverseThrustStateMachine>(FName("ReverseThrustStateMachine"));

	//나이아가라 이펙트 로드
	if (static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Finder(
		TEXT("/Game/VEffects/Free_Fire/Shared/Particles/NS_Fire_Floor_02_Smoke.NS_Fire_Floor_02_Smoke"));
		Finder.Succeeded())
	{
		FireNiagara = Finder.Object;
	}
	
	//체력
	HP = 4000;
}

void AShankBase::BeginPlay()
{
	Super::BeginPlay();

	//체력이 다했을 경우 격추
	OnDeath.AddUFunction(this, FName("OnShotDown"));

	//머터리얼 다이나믹 인스턴스 적용 
	DamageDynamicInstance = SkeletalMeshComp->CreateDynamicMaterialInstance(5);
	
	//1초 뒤에 경로 탐색 상태로 전환
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		SHANK_STATE = EShankState::FindPath;
	}, 1, false);
}

void AShankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}

	if (SHANK_STATE != EShankState::Splash)
	{
		//목표 방향
		const FVector TargetDir = (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const FRotator TargetRot = UKismetMathLibrary::MakeRotFromXZ(TargetDir, GetActorUpVector());
		MeshSceneComp->SetWorldRotation(TargetRot);
	}
}

void AShankBase::SetCurrentState(const EShankState Value)
{
	//다른 상태 머신으로의 전환 요청
	if (CurrentState != Value)
	{
		//임시
		const auto Temp = CurrentStateMachine;

		//상태 전환
		CurrentState = Value;

		//상태 머신 선택
		switch (CurrentState)
		{
		case EShankState::FindPath:
			{
				CurrentStateMachine = FindPathStateMachine;
				break;
			}
		case EShankState::FollowPath:
			{
				CurrentStateMachine = CircleStateMachine;
				break;
			}
		case EShankState::ReverseThrust:
			{
				CurrentStateMachine = ReverseThrustStateMachine;
				break;
			}
		case EShankState::Splash:
			{
				CurrentStateMachine = nullptr;
				break;
			}
		default:
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Not Implemented!"));
			};
		}
		
		//실행 중이던 상태에서 Exit
		if (Temp)
		{
			Temp->ExitState();	
		}
	}
}

void AShankBase::OnAimByPlayerSight()
{
	PRINTLOG(TEXT("FatalError! You Should Fully Override this Interface On Subclass!"));
}

void AShankBase::OnDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (CurrentState == EShankState::Splash)
	{
		return;
	}

	//방어력을 통해 데미지 연산
	const int32 Damage = FMath::FloorToInt(Info.Damage);

	//체력 처리
	HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowNormalDamageActor(GetActorLocation(), Damage);
}

void AShankBase::OnShotDown(const FVector ShotDir)
{
	//상태 전환
	SHANK_STATE = EShankState::Splash;
	
	//추진력 상실, 중력 적용
	DroneMoveComp->Fall();

	//랜덤 방향 충격 적용
	DroneMoveComp->Splash(ShotDir);

	//데미지 파트 가시화
	DamageDynamicInstance->SetScalarParameterValue(FName("opacity"), 1.0f);

	//스폰
	auto Temp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireNiagara, GetRootComponent(), FName("SphereComp"), GetActorLocation(), GetActorRotation(), EAttachLocation::Type::KeepRelativeOffset, true);
	
	//10초 뒤에 소멸
	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, [this]()
	{
		this->Destroy();
	}, 10, false);
}