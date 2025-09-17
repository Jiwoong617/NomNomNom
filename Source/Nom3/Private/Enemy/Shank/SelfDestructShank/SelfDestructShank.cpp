// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/SelfDestructShank/SelfDestructShank.h"
#include "KismetTraceUtils.h"
#include "Core/DamageComponent.h"
#include "Enemy/Shank/Common/ShankDamageComponent.h"
#include "Enemy/Shank/SelfDestructShank/SelfDestructFinalAssaultStateMachine.h"
#include "Enemy/Shank/SelfDestructShank/SelfDestructFindPathStateMachine.h"

ASelfDestructShank::ASelfDestructShank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 로드
	if (static ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(
		TEXT("/Game/Asset/SelfDestructShank/SKM_SelfDestructShank.SKM_SelfDestructShank"));
		Finder.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(Finder.Object);
		SkeletalMeshComp->SetRelativeLocation(FVector(0, 0, -110));
		SkeletalMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	}

	//자폭 생크 전용의 경로 탐색 스테이트 머신 부착
	FindPathStateMachine = CreateDefaultSubobject<USelfDestructFindPathStateMachine>(FName("FindPathStateMachine"));

	//자폭 생크 전용의 최종 돌격 스테이트 머신 부착
	FinalAssaultStateMachine = CreateDefaultSubobject<USelfDestructFinalAssaultStateMachine>(FName("FinalAssaultStateMachine"));
}

void ASelfDestructShank::BeginPlay()
{
	Super::BeginPlay();

	//데미지 컴포넌트 초기화
	DamageComp->Init(ECC_EngineTraceChannel1, FName("Head"), EBodyType::Head);
}

void ASelfDestructShank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		//목표와의 거리가 5m 미만 이라면
		if (const FVector Diff = TargetPawn->GetActorLocation() - GetActorLocation();
			Diff.Length() < 500)
		{
			//최종 돌격 상태 머신으로 전환
			ChangeCurrentStateMachine(FinalAssaultStateMachine);
		}
	}
}

void ASelfDestructShank::OnShotDown(const FVector ShotDir)
{
	Super::OnShotDown(ShotDir);

	//패러미터
	const EObjectTypeQuery ObjType1 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1);
	const EObjectTypeQuery ObjType2 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2);
	TArray<UPrimitiveComponent*> OutComponents;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	//스피어 트레이스
	UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetActorLocation(), 300,
		TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType1, ObjType2 }, DamageComp->StaticClass(), ActorsToIgnore, OutComponents);

	//데미지 정보
	const FFireInfo Info = FFireInfo(2500, GetActorLocation(), ETeamInfo::Enemy, false);
	
	//데미지 컴포넌트
	for (const auto Component : OutComponents)
	{
		if (const auto OtherDamageComp = Cast<UDamageComponent>(Component))
		{
			OtherDamageComp->OnDamaged(Info);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("SelfDestruct!!!"));
}
