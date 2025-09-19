// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShank/ScoutShank.h"
#include "Core/DamageComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/Common/DroneDamageComponent.h"
#include "Enemy/Shank/Common/DroneFollowPathStateMachine.h"
#include "Enemy/Shank/ScoutShank/ScoutShankFindPathStateMachine.h"
#include "Enemy/Shank/ScoutShank/ScoutShankShooterComponent.h"
#include "Kismet/KismetMathLibrary.h"

AScoutShank::AScoutShank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 로드
	if (static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(
		TEXT("/Game/Asset/ScoutShank/SKM_ScoutShank.SKM_ScoutShank"));
		Finder.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(Finder.Object);
		SkeletalMeshComp->SetRelativeLocation(FVector(0, 0, -110));
		SkeletalMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	}

	//일반 데미지 컴포넌트
	DamageComp = CreateDefaultSubobject<UDroneDamageComponent>(FName("DamageComp"));
	DamageComp->SetRelativeLocation(FVector(0, 0, 110));
	DamageComp->SetBoxExtent(FVector(120, 90, 90));
	DamageComp->SetupAttachment(SkeletalMeshComp);

	//정찰 생크 전용의 경로 탐색 스테이트 머신 부착
	FindPathStateMachine = CreateDefaultSubobject<UScoutShankFindPathStateMachine>(FName("FindPathStateMachine"));

	//정찰 생크 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UScoutShankShooterComponent>(FName("ShooterComp"));
	ShooterComp->SetupAttachment(SkeletalMeshComp);
	ShooterComp->SetRelativeLocation(FVector(0, -110, 70));
}

void AScoutShank::BeginPlay()
{
	Super::BeginPlay();

	//자동 사격
	ShooterComp->ActiveAutoFire();

	//데미지 컴포넌트 초기화
	DamageComp->Init(ECC_EngineTraceChannel2, FName("Body"), EBodyType::Body);

	//머터리얼 다이나믹 인스턴스 적용 
	DamageDynamicInstance = SkeletalMeshComp->CreateDynamicMaterialInstance(5);
}

void AScoutShank::OnAimByPlayerSight()
{
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

	//현재 따라가던 경로를 전부 버린다
	PathQueue.Empty();

	//플레이어 폰과 이루는 시선 방향 벡터
	const FVector GazeDir = (GetActorLocation() - TargetPawn->GetActorLocation()).GetSafeNormal();

	//회피 방향을 필터링하는데 사용하는 시선 방향 법선 벡터
	const FVector Element = FVector::DotProduct(GazeDir, TargetPawn->GetActorUpVector()) * GazeDir;
	const FVector GazeUpDir = (GetActorUpVector() - Element).GetSafeNormal();

	//플레이어의 시선과 수직인 방향을 가리키는 방향 벡터
	FVector RandDir = FVector::ZeroVector;

	//45도 라인보다 위에 있으면 아래로 회피, 아래에 있으면 위로 회피하도록
	FOnDecideEvadeDirection Compare;
	if ((FVector::DotProduct(TargetPawn->GetActorUpVector(),GazeDir) > 0.707))
	{
		Compare.BindLambda([this](const float A)
		{
			return A < -0.01;
		});	
	}
	else
	{
		Compare.BindLambda([this](const float A)
		{
			return A > 0.01;
		});
	}

	//목표 방향 결정
	while (true)
	{
		//45도 원뿔에서 랜덤 방향 벡터
		RandDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GazeDir, 45);
		
		//시선 방향 벡터에 대해 정사영 
		RandDir = FVector::VectorPlaneProject(RandDir, GazeDir);

		//정사영 결과가 0에 가깝다면
		if (RandDir.IsNearlyZero())
		{
			continue;
		}

		//시선과 비교했을 때 목표하는 방향에 있다면
		if (Compare.Execute(FVector::DotProduct(RandDir, GazeUpDir)))
		{
			//회피 경로 상에 다른 물체 인식
			TArray<FHitResult> Hits;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			GetWorld()->LineTraceMultiByChannel(Hits, GetActorLocation(), GetActorLocation() + RandDir * 800, ECC_Visibility, QueryParams);

			//적절한 경로 발견
			if (Hits.IsEmpty())
			{
				break;	
			}
		}
	}
	
	//최종 처리
	RandDir = RandDir.GetSafeNormal();

	//추력을 급상승 시켜 회피 운동
	DroneMoveComp->ThrottleToHighManeuver(30);
	
	//랜덤 방향으로 벡터링
	DroneMoveComp->VectorThrust(RandDir);

	//목표 지점 변경
	Destination = GetActorLocation() + RandDir * 800;

	//디버그 라인
	DrawDebugLine(GetWorld(), GetActorLocation(), Destination, FColor::Magenta, false, 3, 0, 0);

	//타이밍
	const float Timing = FMath::RandRange(4, 6);

	//상태 머신 전환
	ChangeCurrentStateMachine(FollowPathStateMachine);	
	
	//무한 회피를 방지하는 타이머 설정
	GetWorldTimerManager().SetTimer(EvadeTimerHandle, [this]()
	{
		//회피 타이머 핸들 초기화
		GetWorldTimerManager().ClearTimer(EvadeTimerHandle);
	}, Timing, false);
}

void AScoutShank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScoutShank::OnShotDown(const FVector ShotDir)
{
	Super::OnShotDown(ShotDir);

	//데미지 파트 가시화
	DamageDynamicInstance->SetScalarParameterValue(FName("opacity"), 1.0f);

	//사격 바활성화
	ShooterComp->InactiveAutoFire();
}