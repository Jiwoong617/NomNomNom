// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Core/EnemyCharacterBase.h"

#include "AITypes.h"
#include "NavigationSystem.h"
#include "Enemy/Core/EnemyHealthComponent.h"
#include "Enemy/Damage/DamageActorPoolWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//배치되어 있거나 스폰되면 AI에 의해 점유
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//AI 컨트롤러 획득
	AIController = Cast<AAIController>(GetController());

	//플레이어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	
	FTimerHandle TimerHandle;
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacterBase::OnAimByPlayerSight()
{
	
}

void AEnemyCharacterBase::OnDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	//일반적인 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowNormalDamageActor(GetActorLocation(), Damage);
}

void AEnemyCharacterBase::OnCriticalDamaged(const FFireInfo Info)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (HealthComp->HP <= 0)
	{
		return;
	}

	//크리티컬 데미지
	const int32 Damage = FMath::FloorToInt(Info.Damage * 2);

	//체력 처리
	HealthComp->HP -= Damage;
	
	//자산의 위치에 데미지 액터 풀링
	DamageActorPool->ShowCriticalDamageActor(GetActorLocation(), Damage);
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

void AEnemyCharacterBase::MoveToTargetLocation(const FVector& TargetLocation) const
{
	//네비게이션 시스템 획득
	auto NS = UNavigationSystemV1::GetCurrent(GetWorld());

	//요청 생성
	FAIMoveRequest Request;
	Request.SetAcceptanceRadius(3);
	Request.SetGoalLocation(TargetLocation);

	//AI가 요청을 이용해 쿼리 구축
	FPathFindingQuery Query;
	AIController->BuildPathfindingQuery(Request, Query);

	//쿼리를 통해 네비게이션 시스템에서 검색
	if (FPathFindingResult FindPathResult = NS->FindPathSync(Query); FindPathResult.Result == ENavigationQueryResult::Type::Success)
	{
		//목표에 도달하는 경로가 있으므로 이동
		AIController->MoveToLocation(TargetPawn->GetActorLocation());
	}
	
	// else
	// {
	// 	//목표에 도달하는 경로가 없으므로 랜덤 위치로 이동할 건데, 이 랜덤에도 문제가 있다면
	// 	if (auto PathFollowResult = AIController->MoveToLocation(RandomLocation);
	// 		PathFollowResult == EPathFollowingRequestResult::AlreadyAtGoal ||
	// 		PathFollowResult == EPathFollowingRequestResult::Failed)
	// 	{
	// 		GetRandomLocationInNavMesh(TargetPawn->GetActorLocation(), 500, RandomLocation);
	// 	}
	// }
}

bool AEnemyCharacterBase::GetRandomLocationInNavMesh(const FVector& CenterLocation, const float Radius, FVector& Destination) const
{
	//네비게이션 시스템 획득
	const auto NS = UNavigationSystemV1::GetCurrent(GetWorld());

	//네비게이션 시스템을 통해서 도달 가능한 랜덤 위치 획득
	FNavLocation RandomReachableLocation;
	const bool bResult = NS->GetRandomReachablePointInRadius(CenterLocation, Radius, RandomReachableLocation);
	Destination = RandomReachableLocation.Location;

	//성공 결과
	return bResult;
}