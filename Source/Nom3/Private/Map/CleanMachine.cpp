// Fill out your copyright notice in the Description page of Project Settings.


#include "Nom3/Public/Map/CleanMachine.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/NomPlayer.h"


// Sets default values
ACleanMachine::ACleanMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CleanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clean Mesh"));
	RootComponent = CleanMesh;
	HitPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Point"));
	HitPoint->SetupAttachment(CleanMesh);
	
}

void ACleanMachine::OnInteract(AActor OtherActor)
{
	auto player = Cast<ANomPlayer>(GetOwner());
	// 나중에 player hp 정의 player -> --MaxHP;
}

// Called when the game starts or when spawned
void ACleanMachine::BeginPlay()
{
	Super::BeginPlay();

	if (TargetSpline)
	{
		TargetSpline = TargetSplineActor->FindComponentByClass<USplineComponent>();
	}
	if (!TargetSpline)
	{
		SetActorTickEnabled(false);
		UE_LOG(LogTemp,Warning,TEXT("없네요 까비쓰")); 
		return;
		
	}
	
}



// Called every frame
void ACleanMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	switch (normalstate)
	{
	case ECleanMState::Chasing:
		break;
	case ECleanMState::FollowingSpline:
		break;
	case ECleanMState::TargetLockOn:
		break;
	}
	
	
	
}

void ACleanMachine::TraceMove(float DeltaTime)
{
	FVector p0 = GetActorLocation();
	FVector vt = GetActorForwardVector() * speed * DeltaTime;
	FVector P = p0 + vt;

	const FVector location = GetActorLocation();
	const FVector forward = GetActorForwardVector();

	const FVector TargetLocation = TargetSpline->GetLocationAtDistanceAlongSpline(0 + 300.f, ESplineCoordinateSpace::World);
	
}

// 스플라인 따라갈떄 해머 무게추 달기
