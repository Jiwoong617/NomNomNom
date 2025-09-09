// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankBase.h"
#include "Enemy/Shank/ShankFollowPathStateMachine.h"
#include "Components/SphereComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Enemy/Damage/DamageActorPoolGameInstanceSubsystem.h"
#include "Enemy/Shank/ShankFindPathStateMachine.h"
#include "Enemy/Shank/ShankReverseThrustStateMachine.h"
#include "Kismet/KismetMathLibrary.h"

AShankBase::AShankBase() :
	KeepDistance(150),
	CurrentState(EShankState::Sleep)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 컴포넌트
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	//노말 히트 컴포넌트
	NormalHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("NormalHitComp"));
	NormalHitComp->SetupAttachment(SphereComp);

	//크리티컬 히트 컴포넌트
	CriticalHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("CriticalHitComp"));
	CriticalHitComp->SetupAttachment(SphereComp);

	//스켈레탈 컴포넌트
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(SphereComp);

	//드론 무브먼트 컴포넌트
	DroneMoveComp = CreateDefaultSubobject<UDroneMovementComponent>(TEXT("DroneMoveComp"));

	//경로 탐색 상태 머신
	FindPathStateMachine = CreateDefaultSubobject<UShankFindPathStateMachine>(TEXT("FindPathStateMachine"));
	
	//경로 추적 상태 머신
	CircleStateMachine = CreateDefaultSubobject<UShankFollowPathStateMachine>(TEXT("CircleStateMachine"));

	//역추진 상태 머신
	ReverseThrustStateMachine = CreateDefaultSubobject<UShankReverseThrustStateMachine>(TEXT("ReverseThrustStateMachine"));
	
	//체력
	HP = 10000;

	//방어력
	ARMOR = 0.9;
}

void AShankBase::BeginPlay()
{
	Super::BeginPlay();

	//체력이 다했을 경우 격추
	OnDeath.AddUFunction(this, FName("OnShotDown"));

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

		//목표 지점으로 가는 경로를 디버그 드로우
		DrawDebugLine(GetWorld(), GetActorLocation(), TargetLocation, FColor::Blue, false, -1, 0, 0);
	}
}

void AShankBase::OnAimByPlayerSight()
{
	//선회 상태가 아니라면
	if (CurrentState != EShankState::FollowPath)
	{
		return;
	}
	
	//회피 후 시간이 충분히 지나지 않았다면
	if (DodgeTimerHandle.IsValid())
	{
		return;
	}

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
	DroneMoveComp->ThrottleHighToEvade();
	
	//랜덤 방향으로 벡터링
	DroneMoveComp->VectorThrust(RandDir);

	//목표 지점 변경
	TargetLocation = GetActorLocation() + RandDir * 800;

	//타이밍
	const float Timing = FMath::RandRange(5, 10);
	
	//무한 회피를 방지하는 타이머 설정
	GetWorldTimerManager().SetTimer(DodgeTimerHandle, [this]()
	{
		//회피 타이머 핸들 초기화
		GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	}, Timing, false);
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

void AShankBase::OnShotByPlayer(const FVector ShotDir, const int Attack)
{
	//격추 상태에서는 더 이상 피격될 수 없다
	if (CurrentState == EShankState::Splash)
	{
		return;
	}

	//방어력을 통해 데미지 연산
	const int32 Damage = FMath::FloorToInt(static_cast<float>(Attack) * (1.0 - ARMOR));

	//체력 처리
	HP -= Damage;
	
	//데미지 출력
	DamageActorPool->ShowNormalDamageActor(GetActorLocation(), Damage);
}

void AShankBase::OnShotDown(const FVector ShotDir)
{
	//상태 전환
	CurrentState = EShankState::Splash;
	
	//추진력 상실, 중력 적용
	DroneMoveComp->Fall();

	//랜덤 방향 충격 적용
	DroneMoveComp->Splash(ShotDir);

	//명중 콜라이더 비활성화
	NormalHitComp->SetCollisionProfileName("NoCollision");
	CriticalHitComp->SetCollisionProfileName("NoCollision");
	
	//10초 뒤에 소멸
	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, [this]()
	{
		this->Destroy();
	}, 10, false);
}