// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Human/Common/HumanBase.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Human/Common/HumanEvadeStateMachine.h"
#include "Enemy/Human/Common/HumanIdleStateMachine.h"
#include "Enemy/Human/Common/HumanMoveStateMachine.h"

AHumanBase::AHumanBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시 씬 컴포넌트
	MeshSceneComp = CreateDefaultSubobject<USceneComponent>(FName("MeshSceneComp"));
	MeshSceneComp->SetupAttachment(RootComponent);

	//체력 컴포넌트
	HealthComp = CreateDefaultSubobject<UEnemyHealthComponent>(FName("HealthComp"));

	//스테이터스 위젯 컴포넌트
	StatusWidgetComp->SetRelativeLocation(FVector(0, 0, 150));
	StatusWidgetComp->SetupAttachment(GetCapsuleComponent());

	//대기 상태 머신
	IdleStateMachine = CreateDefaultSubobject<UHumanIdleStateMachine>(FName("IdleStateMachine"));

	//이동 상태 머신
	MoveStateMachine = CreateDefaultSubobject<UHumanMoveStateMachine>(FName("MoveStateMachine"));

	//회피 상태 머신
	EvadeStateMachine = CreateDefaultSubobject<UHumanEvadeStateMachine>(FName("EvadeStateMachine"));

	//사망 나이아가라 시스템 로드
	if (static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		Finder(TEXT("/Game/MsvFx_Niagara_Explosion_Pack_01/Prefabs/NS_Die.NS_Die"));
		Finder.Succeeded())
	{
		DieNiagara = Finder.Object;	
	}
}

void AHumanBase::OnNoticePawn(AActor* DetectedPawn)
{
	Super::OnNoticePawn(DetectedPawn);

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), *DetectedPawn->GetActorLabel()));
	
	//상태 머신 전환
	ChangeCurrentStateMachine(IdleStateMachine);
}

void AHumanBase::BeginPlay()
{
	Super::BeginPlay();
	
	//사망 메서드 바인드
	HealthComp->OnDeath.AddUFunction(this, FName("OnDie"));
}

void AHumanBase::OnDie()
{
	Super::OnDie();
	
	//상태 전환
	ChangeCurrentStateMachine(nullptr);

	//스폰
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DieNiagara, GetActorLocation(), GetActorRotation(), FVector(1), true);

	//이동 명령 정지
	AIController->StopMovement();

	//랙돌
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();

	//10초 뒤에 소멸
	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, [this]()
	{
		this->Destroy();
	}, 10, false);
}

void AHumanBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//현재 상태 머신 실행
		CurrentStateMachine->ExecuteState();
	}
}

bool AHumanBase::FindReachableLocation(FVector& TargetLocation) const
{
	//실패할 경우 1m 범위 내 도달 가능한 지점을 랜덤으로 획득
	FNavLocation ReachableLocation;
	for (int32 i = 1; i <= 100; i++)
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