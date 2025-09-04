// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ShankBase.h"

#include "Components/SphereComponent.h"
#include "Enemy/DroneMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nom3/Nom3.h"

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

	//플레이어 폰의 머리 원뿔 어딘가를 목표로 잡고
	const FVector RandDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetActor->GetActorUpVector(), 75);
	TargetLocation = TargetActor->GetActorLocation() + RandDir * FMath::FRandRange(KeepDistance - 50, KeepDistance + 50);
	
	// FTimerHandle Handle;
	// GetWorldTimerManager().SetTimer(Handle, [this]()
	// {
	// 	//플레이어 폰의 머리 원뿔 어딘가를 목표로 잡고
	// 	const FVector RandDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetActor->GetActorUpVector(), 45);
	// 	TargetLocation = TargetActor->GetActorLocation() + RandDir * FMath::FRandRange(KeepDistance - 50, KeepDistance + 50);
	// }, 10, true);
}

void AShankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//목표까지의 차이 벡터
	const FVector TargetDiff = TargetLocation - GetActorLocation();

	//목표까지의 거리가 10m 이내라면
	if (const float Length2Target = TargetDiff.Length(); Length2Target < 100)
	{
		//추진력을 조절한다
		const float Value = ThrottleControlCurve->GetFloatValue(Length2Target / 100);
		DroneMoveComp->ThrottleThrust(Value);
	}
	
	//목표로 향하는 방향
	const FVector TargetDir = TargetDiff.GetSafeNormal();
		
	//추력 편향
	DroneMoveComp->VectorThrust(TargetDir);		
}

