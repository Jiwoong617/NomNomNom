// Fill out your copyright notice in the Description page of Project Settings.


#include "Nom3/Public/Map/CleanMachine.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/NomPlayer.h"
#include "Nom3/Nom3.h"


// Sets default values
ACleanMachine::ACleanMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CleanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clean Mesh"));
	RootComponent = CleanMesh;
	HitPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Point"));
	HitPoint->SetupAttachment(CleanMesh);
	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->SetupAttachment(RootComponent);

	normalstate = ECleanMState::FollowingSpline;
	
	
}

void ACleanMachine::OnInteract(AActor OtherActor)
{
	auto player = Cast<ANomPlayer>(GetOwner());
	//player->InputComponent->BindAction<UEnhancedInputComponent>();
	if (player)
	{
		// player-> bOncompoverlap = --MaxHp 
	}
}

// Called when the game starts or when spawned
void ACleanMachine::BeginPlay()
{

	
	//  이런식으로 사용 가능하다 PRINTLOG(TEXT("목표 좌표 : %d %f %f %f %s"), 5, 1., .2, .3, TEXT("aksjdf"));
	if (!TargetSplineActor)
	{
		
		SetActorTickEnabled(false);
		return;
	}

	TargetSpline = TargetSplineActor->FindComponentByClass<USplineComponent>();
	if (!TargetSpline)
	{
		
		SetActorTickEnabled(false);
		return;
	}
	
	SetActorLocation(TargetSpline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
}



// Called every frame
void ACleanMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (TargetSpline)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Tick 실행중! Dist=%f, SplineLength=%f"),
	// 		DistanceAlongSpline, TargetSpline->GetSplineLength());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("TargetSpline이 nullptr 입니다!"));
	// }

	if (!TargetSpline || TargetSpline->GetSplineLength() <= 0.0f)
	{
		return;
	}

	DistanceAlongSpline += speed * DeltaTime;

	if (DistanceAlongSpline > TargetSpline->GetSplineLength())
	{
		DistanceAlongSpline = 0.0f;
	}

	const FVector NewLocation = TargetSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	// 먼저 이동만 확인
	SetActorLocation(NewLocation);

	// 회전은 나중에 확인
	//const FRotator NewRotation = TargetSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	//SetActorRotation(NewRotation);
}


void ACleanMachine::TraceMove(float DeltaTime)
{
	FVector p0 = GetActorLocation();
	FVector vt = GetActorForwardVector() * speed * DeltaTime;
	FVector P = p0 + vt ;

	const FVector location = GetActorLocation();
	const FVector forward = GetActorForwardVector();

	const FVector TargetLocation = TargetSpline->GetLocationAtDistanceAlongSpline(0 + 300.f, ESplineCoordinateSpace::World);
	
}

void ACleanMachine::FollwSpline(float DeltaTime)
{
	if (!TargetSpline) return;

	const float SplineLength = TargetSpline->GetSplineLength();
	if (SplineLength <= 0.0f) return;

	DistanceAlongSpline += speed * DeltaTime;
	UE_LOG(LogTemp, Log, TEXT("FollwSpline 실행 중: 현재 이동 거리는 = %f"), DistanceAlongSpline);

	if (DistanceAlongSpline > SplineLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollwSpline: 스플라인 끝에 도달하여 처음으로 돌아갑니다."));
		DistanceAlongSpline = 0.0f;
	}

	const FVector NewLocation = TargetSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	SetActorLocation(NewLocation);
}

// 스플라인 따라갈떄 해머 무게추 달기
