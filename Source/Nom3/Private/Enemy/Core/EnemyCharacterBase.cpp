// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyCharacterBase.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "Enemy/Core/EnemyData.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Core/StateMachineBase.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Core/EnemyStatus.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//배치되어 있거나 스폰되면 AI에 의해 점유
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//위젯 컴포넌트 부착
	if (static ConstructorHelpers::FClassFinder<UEnemyStatus>
		Finder(TEXT("/Game/Enemies/Default/WBP_EnemyStatus.WBP_EnemyStatus_C"));
		Finder.Class)
	{
		StatusWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("StatusWidgetComp"));
		StatusWidgetComp->SetWidgetClass(Finder.Class);
		StatusWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		StatusWidgetComp->SetDrawSize(FVector2D(150, 10));
	}
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//스테이터스 위젯 획득
	StatusWidget = Cast<UEnemyStatus>(StatusWidgetComp->GetWidget());
	StatusWidget->Hide();

	//스테이터스 위젯 업데이트
	StatusWidget->SetName(EnemyData->EnemyName);

	//체력 초기화
	HealthComp->Init(EnemyData->EnemyMaxHP);

	//원하는 AI 컨트롤러 스폰
	SpawnDefaultController();

	//AI 컨트롤러 획득
	AIController = Cast<AAIController>(GetController());

	//네비게이션 시스템 획득
	NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	//데미지 액터 풀링 서브시스템 획득
	if (auto Temp = GetWorld()->GetSubsystem<UDamageActorPoolWorldSubsystem>())
	{
		DamageActorPool = Temp;
	}
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UStateMachineBase* AEnemyCharacterBase::GetCurrentStateMachine() const
{
	return CurrentStateMachine;
}

void AEnemyCharacterBase::ChangeCurrentStateMachine(UStateMachineBase* StateMachineToChange)
{
	//임시 저장
	const auto Before = CurrentStateMachine;

	//스테이트 머신 변경
	CurrentStateMachine = StateMachineToChange;

	//탈출
	if (Before)
	{
		Before->ExitState();	
	}
}

void AEnemyCharacterBase::OnAimByPlayerSight()
{
	if (GetCurrentStateMachine() == nullptr)
	{
		return;
	}
	
	//스테이터스 위젯 가시화
	StatusWidget->Show();

	//2초 후에 스테이터스 위젯 비가시화 예약, 다시 들어온다면 덮어쓰니까 문제 없음
	GetWorldTimerManager().SetTimer(OnSightTimerHandle, [this]()
	{
		StatusWidget->Hide();
	}, 2, false);
}

void AEnemyCharacterBase::OnDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	if (TargetPawn == nullptr)
	{
		//폰 획득
		OnNoticePawn(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));	
	}

	//일반적인 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowNormalDamageActor(GetActorLocation(), Damage);

	//체력 비율 업데이트
	StatusWidget->UpdateHPBar(HealthComp->GetHPPercent());
}

void AEnemyCharacterBase::OnCriticalDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	if (TargetPawn == nullptr)
	{
		//폰 획득
		OnNoticePawn(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));	
	}

	//크리티컬 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage * 2);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowCriticalDamageActor(GetActorLocation(), Damage);

	//체력 비율 업데이트
	StatusWidget->UpdateHPBar(HealthComp->GetHPPercent());
}

void AEnemyCharacterBase::OnNoticePawn(AActor* DetectedPawn)
{
	//이미 목표 폰을 가지고 있다면
	if (TargetPawn)
	{
		return;
	}
	
	//목표 폰 전달
	TargetPawn = DetectedPawn;

	//스켈레탈 메시의 애니메이션 모드 변경
	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBlueprintClass);
}

void AEnemyCharacterBase::OnDie()
{
	//스테이터스 위젯 비가시화
	StatusWidget->Hide();
}

FVector AEnemyCharacterBase::GetPlayerGazeDir() const
{
	return (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
}

FVector AEnemyCharacterBase::GetPlayerGazeUpDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	const FVector Element = FVector::DotProduct(PlayerGazeDir, TargetPawn->GetActorUpVector()) * PlayerGazeDir;
	return (TargetPawn->GetActorUpVector() - Element).GetSafeNormal();
}

FVector AEnemyCharacterBase::GetPlayerGazeRightDir() const
{
	const FVector PlayerGazeDir = GetPlayerGazeDir();
	return FVector::CrossProduct(TargetPawn->GetActorUpVector(), PlayerGazeDir).GetSafeNormal();
}