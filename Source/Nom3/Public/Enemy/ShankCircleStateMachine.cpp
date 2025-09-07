// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/ShankCircleStateMachine.h"
#include "DroneMovementComponent.h"
#include "ShankBase.h"

UShankCircleStateMachine::UShankCircleStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UShankCircleStateMachine::BeginPlay()
{
	Super::BeginPlay();

	//유효한 생크에 부착됐다면
	if (auto Temp = Cast<AShankBase>(GetOwner()))
	{
		OwnerShank = Temp;
	}

	//델리게이트 바인딩
	DecideTargetLocationDelegate.BindUFunction(this, FName("DecideTargetLocation"));
}

void UShankCircleStateMachine::EnterState()
{
	Super::EnterState();
	
	if (OwnerShank == false)
	{
		return;
	}

	//최초 목표 위치
	DecideTargetLocationDelegate.Execute();

	//목표 위치를 수시로 변경
	GetWorld()->GetTimerManager().SetTimer(CirclingTimerHandle, DecideTargetLocationDelegate, 10, true);
}

void UShankCircleStateMachine::ExecuteState()
{
	Super::ExecuteState();
	
	if (OwnerShank == false)
	{
		return;
	}

	//진입 이벤트 발생
	if (bReadyToExecute == false)
	{
		EnterState();
	}

	//목표까지의 차이 벡터
	const FVector TargetDiff = OwnerShank->TargetLocation - OwnerShank->GetActorLocation();
		
	//목표까지의 거리에 따라 추력 조절
	if (const float Length2Target = TargetDiff.Length(); Length2Target < 100)
	{
		//목표까지의 거리가 10m 이내라면
		const float Value = OwnerShank->ThrottleControlCurve->GetFloatValue(Length2Target / 100);
		OwnerShank->DroneMoveComp->ThrottleThrustByLevel(Value);
	}
	else
	{
		//목표까지의 거리가 10m 밖이라면
		OwnerShank->DroneMoveComp->ThrottleToCruiseThrust();
	}
		
	//목표로 향하는 방향
	const FVector TargetDir = TargetDiff.GetSafeNormal();
		
	//추력 편향
	OwnerShank->DroneMoveComp->VectorThrust(TargetDir);
	
	//목표 지점 디버그 드로우
	DrawDebugLine(GetWorld(), OwnerShank->GetActorLocation(), OwnerShank->TargetLocation, FColor::Blue, false, -1, 0, 0);
}

void UShankCircleStateMachine::ExitState()
{
	Super::ExitState();
	
	if (OwnerShank == false)
	{
		return;
	}

	//타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(CirclingTimerHandle);
}

void UShankCircleStateMachine::DecideTargetLocation() const
{
	//목표의 방향 요소 벡터
	const FVector UpDir = OwnerShank->TargetPawn->GetActorUpVector();
	const FVector RightDir = OwnerShank->TargetPawn->GetActorRightVector();
	
	//북극에서 적도를 향해 랜덤 범위만큼
	FVector RandDir = UpDir.RotateAngleAxis(FMath::RandRange(60, 85), RightDir);
	
	//북극축을 기준으로 랜덤 범위만큼 회전
	RandDir = RandDir.RotateAngleAxis(FMath::RandRange(-180, 180), UpDir);
	
	//적당한 거리에 적당한 눈높이에 있는 360도 범위 내 좌표를 가지게 된다
	const float Distance = OwnerShank->KeepDistance;
	OwnerShank->TargetLocation = OwnerShank->TargetPawn->GetActorLocation() + RandDir * FMath::FRandRange(Distance - 50, Distance + 50);
}

void UShankCircleStateMachine::ResetCirclingTimerHandle(const float Timing)
{
	//타이머 핸들 클리어
	GetWorld()->GetTimerManager().ClearTimer(CirclingTimerHandle);

	//잠시 대기한 뒤 선회 재시작
	FTimerHandle Temp;
	GetWorld()->GetTimerManager().SetTimer(Temp, [this]()
	{
		//목표 위치를 수시로 변경
		GetWorld()->GetTimerManager().SetTimer(CirclingTimerHandle, DecideTargetLocationDelegate, 10, true);
	}, Timing, false);
}