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
	UE_LOG(LogTemp, Error, TEXT("--- 최종 디버깅 시작 ---"));
	

	if (!TargetSplineActor)
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay 실패: [원인 1] 에디터에서 TargetSplineActor가 지정되지 않았습니다!"));
		SetActorTickEnabled(false);
		return;
	}

	TargetSpline = TargetSplineActor->FindComponentByClass<USplineComponent>();
	if (!TargetSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay 실패: [원인 2] 지정된 TargetSplineActor에 Spline 컴포넌트가 없습니다!"));
		SetActorTickEnabled(false);
		return;
	}

	const float SplineLength = TargetSpline->GetSplineLength();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay 성공: 스플라인을 찾았습니다! 총 길이는: %f"), SplineLength);

	if (SplineLength <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay 경고: [원인 3] 스플라인의 길이가 0입니다! 경로를 그려주세요."));
	}

	if (speed <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay 경고: [원인 4] speed 값이 0 또는 음수입니다!"));
	}

	SetActorLocation(TargetSpline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
}



// Called every frame
void ACleanMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PathSpline || PathSpline->GetSplineLength() <= 0.0f)
	{
		return;
	}

	// 스플라인을 따라 이동 거리 업데이트
	DistanceAlongSpline += speed * DeltaTime;

	// 스플라인 끝에 도달하면 처음으로 리셋
	if (DistanceAlongSpline > PathSpline->GetSplineLength())
	{
		DistanceAlongSpline = 0.0f;
	}

	// 새로운 위치와 회전값 계산
	const FVector NewLocation = PathSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	const FRotator NewRotation = PathSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	// 액터 위치 및 회전 업데이트
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
	
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
