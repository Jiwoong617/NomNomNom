// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ShankBase.h"
#include "Enemy/Shank/ShankCircleStateMachine.h"
#include "Components/SphereComponent.h"
#include "Enemy/Components/DroneMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

	//선회 상태 머신
	CircleStateMachine = CreateDefaultSubobject<UShankCircleStateMachine>(TEXT("CircleStateMachine"));
	
	
}

void AShankBase::BeginPlay()
{
	Super::BeginPlay();

	//플레어어 폰 획득
	TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//2초 뒤에 선회 상태로 전환
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		this->CurrentState = EShankState::Circle;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Circle!"));
	}, 2, false);
}

void AShankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EShankState::Circle:
		{
			CircleStateMachine->ExecuteState();
			break;
		}
	default:
		{
			break;
		}
	}
}

void AShankBase::OnAimByPlayerSight()
{
	//선회 상태가 아니라면
	if (CurrentState != EShankState::Circle)
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
	FVector NormalToGazeDir = FVector::ZeroVector;

	//30도선보다 위에 있으면 아래로 회피, 아래에 있으면 위로 회피하도록
	FOnDecideEvadeDirection Compare;
	if ((FVector::DotProduct(TargetPawn->GetActorUpVector(),GazeDir) > 0.34))
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
	
	while (true)
	{
		//45도 원뿔에서 랜덤 방향 벡터
		NormalToGazeDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GazeDir, 45);
		
		//시선 방향 벡터에 대해 정사영 
		NormalToGazeDir = FVector::VectorPlaneProject(NormalToGazeDir, GazeDir);

		//시선과 비교했을 때 상단에 있다면 반복문 종료
		if (Compare.Execute(FVector::DotProduct(NormalToGazeDir, GazeUpDir)))
		{
			//자연스럽게 시선 벡터와 일치하는 경우도 걸러진다
			break;
		}
	}
	
	//최종 처리
	NormalToGazeDir = NormalToGazeDir.GetSafeNormal();

	//추력을 급상승 시켜 회피 운동
	DroneMoveComp->ThrottleHighToEvade();
	
	//랜덤 방향으로 벡터링
	DroneMoveComp->VectorThrust(NormalToGazeDir);

	//목표 지점 변경
	TargetLocation = GetActorLocation() + NormalToGazeDir * FMath::RandRange(400, 800);

	//타이밍
	const float Timing = FMath::RandRange(2, 4);
	
	//무한 회피를 방지하는 타이머 설정
	GetWorldTimerManager().SetTimer(DodgeTimerHandle, [this]()
	{
		//회피 타이머 핸들 초기화
		GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	}, Timing, false);

	//선회 타이머 핸들 재시작
	CircleStateMachine->ResetCirclingTimerHandle(Timing);
}

void AShankBase::OnShotByPlayer(const FVector ShotDir, int Damage)
{
	OnShotDown(ShotDir);
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