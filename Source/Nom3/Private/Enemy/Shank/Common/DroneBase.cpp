// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/DroneBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Shank/Common/DroneFollowPathStateMachine.h"
#include "Enemy/Shank/Common/DroneReverseThrustStateMachine.h"
#include "Kismet/KismetMathLibrary.h"

ADroneBase::ADroneBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 컴포넌트
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	SetRootComponent(SphereComp);

	//스테이터스 위젯 컴포넌트
	StatusWidgetComp->SetRelativeLocation(FVector(0, 0, 150));
	StatusWidgetComp->SetupAttachment(SphereComp);
	
	//메시 씬 컴포넌트
	MeshSceneComp = CreateDefaultSubobject<USceneComponent>(FName("MeshSceneComp"));
	MeshSceneComp->SetupAttachment(RootComponent);

	//스켈레탈 컴포넌트
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComp"));
	SkeletalMeshComp->SetCollisionProfileName(FName("NoCollision"));
	SkeletalMeshComp->SetupAttachment(MeshSceneComp);

	//드론 무브먼트 컴포넌트
	DroneMoveComp = CreateDefaultSubobject<UDroneMovementComponent>(FName("DroneMoveComp"));
	
	//경로 추적 상태 머신
	FollowPathStateMachine = CreateDefaultSubobject<UDroneFollowPathStateMachine>(FName("CircleStateMachine"));

	//역추진 상태 머신
	ReverseThrustStateMachine = CreateDefaultSubobject<UDroneReverseThrustStateMachine>(FName("ReverseThrustStateMachine"));

	//나이아가라 시스템 로드
	if (static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Finder(
		TEXT("/Game/MsvFx_Niagara_Explosion_Pack_01/Prefabs/Niagara_Explosion_06.Niagara_Explosion_06"));
		Finder.Succeeded())
	{
		ExplosionNiagara = Finder.Object;
	}
}

void ADroneBase::BeginPlay()
{
	Super::BeginPlay();

	//사망 메서드 바인드
	HealthComp->OnDeath.AddUFunction(this, FName("OnDie"));

	//시뮬레이션 개시
	DroneMoveComp->Simulate();
	
	//1초 뒤에 경로 탐색 상태로 전환
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		ChangeCurrentStateMachine(FindPathStateMachine);
	}, 1, false);
}

void ADroneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 생크 상태 머신 실행
		CurrentStateMachine->ExecuteState();

		//목표 방향
		const FVector TargetDir = (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const FRotator TargetRot = UKismetMathLibrary::MakeRotFromXZ(TargetDir, GetActorUpVector());
		MeshSceneComp->SetWorldRotation(TargetRot);
	}
}

void ADroneBase::OnDie()
{
	Super::OnDie();

	//상태 전환
	ChangeCurrentStateMachine(nullptr);
	
	//추진력 상실, 중력 적용
	DroneMoveComp->Fall();

	//랜덤 방향 충격 적용
	DroneMoveComp->Splash(FMath::VRand());
	
	//10초 뒤에 소멸
	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, [this]()
	{
		this->Destroy();
	}, 10, false);
}