// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ShankBase.h"

#include "Components/SphereComponent.h"
#include "Enemy/DroneMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AShankBase::AShankBase() : KeepDistance(150)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

void AShankBase::BeginPlay()
{
	Super::BeginPlay();

	//플레어어 폰 획득
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//목표 위치 결정 델리게이트
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, FName("DecideTargetLocation"));

	//최초 목표 위치
	Delegate.Execute();

	//목표 위치를 수시로 변경
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, Delegate, 10, true);
}

void AShankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//목표까지의 차이 벡터
	const FVector TargetDiff = TargetLocation - GetActorLocation();
	
	//목표까지의 거리에 따라 추력 조절
	if (const float Length2Target = TargetDiff.Length(); Length2Target < 100)
	{
		//목표까지의 거리가 10m 이내라면
		const float Value = ThrottleControlCurve->GetFloatValue(Length2Target / 100);
		DroneMoveComp->ThrottleThrustByLevel(Value);
	}
	else
	{
		//목표까지의 거리가 10m 밖이라면
		DroneMoveComp->ThrottleToCruiseThrust();
	}
	
	//목표로 향하는 방향
	const FVector TargetDir = TargetDiff.GetSafeNormal();
		
	//추력 편향
	DroneMoveComp->VectorThrust(TargetDir);		
}

void AShankBase::DecideTargetLocation()
{
	//목표의 벡터
	const FVector UpDir = TargetActor->GetActorUpVector();
	const FVector RightDir = TargetActor->GetActorRightVector();

	//북극에서 적도를 향해 랜덤 범위만큼
	FVector RandDir = UpDir.RotateAngleAxis(FMath::RandRange(45, 75), RightDir);

	//북극축을 기준으로 랜덤 범위만큼 회전
	RandDir = RandDir.RotateAngleAxis(FMath::RandRange(-180, 180), UpDir);
	
	//적당한 거리에 적당한 눈높이에 있는 360도 범위 내 좌표를 가지게 된다
	TargetLocation = TargetActor->GetActorLocation() + RandDir * FMath::FRandRange(KeepDistance - 50, KeepDistance + 50);	
	DrawDebugLine(GetWorld(), GetActorLocation(), TargetLocation, FColor::Red, false, 10, 0, 0);
}

void AShankBase::OnAimByPlayerSight()
{
	//회피 후 시간이 충분히 지나지 않았다면
	if (DodgeTimerHandle.IsValid())
	{
		return;
	}

	//추력을 급상승 시켜 회피 운동
	DroneMoveComp->ThrottleHighToEvade();

	//랜덤 방향으로 벡터링
	const FVector RandDir = FMath::VRand();
	DroneMoveComp->VectorThrust(RandDir);

	//목표 지점 변경
	const FVector RandLocation = GetActorLocation() + RandDir * FMath::RandRange(400, 800);
	TargetLocation = RandLocation;

	//무한 회피를 방지하는 타이머 설정
	GetWorldTimerManager().SetTimer(DodgeTimerHandle, [this]()
	{
		//타이머 핸들 초기화
		GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	}, 2, false);
}

void AShankBase::OnShotByPlayer(int Damage)
{
	OnShotDown();
}

void AShankBase::OnShotDown()
{
	DroneMoveComp->Fall();
	DroneMoveComp->Shock();
}
