// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShank.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Shank/ScoutShankDamageComponent.h"
#include "Enemy/Shank/ScoutShankShooterComponent.h"
#include "Enemy/Shank/ShankFollowPathStateMachine.h"
#include "Kismet/KismetMathLibrary.h"

AScoutShank::AScoutShank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//정찰 생크 전용의 사격 컴포넌트 부착
	ShooterComp = CreateDefaultSubobject<UScoutShankShooterComponent>(TEXT("ShooterComp"));
	ShooterComp->SetupAttachment(GetRootComponent());
	ShooterComp->SetRelativeLocation(FVector(0, 0, -50));

	//일반 데이지 컴포넌트
	NormalDamageComp = CreateDefaultSubobject<UScoutShankDamageComponent>(TEXT("NormalDamageComp"));
	NormalDamageComp->SetupAttachment(RootComponent);
}

void AScoutShank::BeginPlay()
{
	Super::BeginPlay();

	//자동 사격
	ShooterComp->ActiveAutoFire();

	//데미지 컴포넌트 초기화
	NormalDamageComp->Init(ECC_EngineTraceChannel1, FName("Body"), EBodyType::Body);
}

void AScoutShank::OnAimByPlayerSight()
{
	//회피가 불가능한 상태가 아니라면
	if (CurrentState == EShankState::Splash || CurrentState == EShankState::Sleep)
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
	TargetLocation = GetActorLocation() + RandDir * 800;

	//타이밍
	const float Timing = FMath::RandRange(2, 4);

	//상태 머신 전환
	SHANK_STATE = EShankState::FollowPath;
	
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

	//비활성화
	NormalDamageComp->Inactive();
}