// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/SelfDestructShank/SelfDestructShank.h"
#include "KismetTraceUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/DamageComponent.h"
#include "Enemy/Shank/Common/DroneDamageComponent.h"
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

	//크리티컬 데미지 컴포넌트
	CriticalDamageComp = CreateDefaultSubobject<UDroneDamageComponent>(FName("CriticalDamageComp"));
	CriticalDamageComp->SetRelativeLocation(FVector(0, 0, 110));
	CriticalDamageComp->SetBoxExtent(FVector(120, 90, 90));
	CriticalDamageComp->SetupAttachment(SkeletalMeshComp);

	//자폭 생크 전용의 경로 탐색 스테이트 머신 부착
	FindPathStateMachine = CreateDefaultSubobject<USelfDestructFindPathStateMachine>(FName("FindPathStateMachine"));

	//자폭 생크 전용의 최종 돌격 스테이트 머신 부착
	FinalAssaultStateMachine = CreateDefaultSubobject<USelfDestructFinalAssaultStateMachine>(FName("FinalAssaultStateMachine"));
}

void ASelfDestructShank::BeginPlay()
{
	Super::BeginPlay();

	//데미지 컴포넌트 초기화
	CriticalDamageComp->Init(ECC_EngineTraceChannel1, FName("Head"), EBodyType::Head);

	//머터리얼 다이나믹 인스턴스 적용 
	DamageDynamicInstance = SkeletalMeshComp->CreateDynamicMaterialInstance(9);
}

void ASelfDestructShank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//유효한 상태 머신이 설정되어 있다면
	if (CurrentStateMachine)
	{
		if (CurrentStateMachine != FinalAssaultStateMachine)
		{
			//목표와의 거리가 15m 미만 이라면
			if (const FVector Diff = TargetPawn->GetActorLocation() - GetActorLocation();
				Diff.Length() < 1500)
			{
				//최종 돌격 상태 머신으로 전환
				ChangeCurrentStateMachine(FinalAssaultStateMachine);
			}	
		}
	}
}

void ASelfDestructShank::OnShotDown(const FVector ShotDir)
{
	Super::OnShotDown(ShotDir);

	//다이나믹 머터리얼 인스턴스
	DamageDynamicInstance->SetScalarParameterValue(FName("Opacity"), 0);

	//스폰
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionNiagara, GetActorLocation(), GetActorRotation(), FVector(2), true);

	//패러미터
	const EObjectTypeQuery ObjType1 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1);
	const EObjectTypeQuery ObjType2 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2);
	TArray<UPrimitiveComponent*> OutComponents;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	//스피어 트레이스
	UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetActorLocation(), 750,
		TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType1, ObjType2 }, nullptr, ActorsToIgnore, OutComponents);

	//데미지 정보
	const FFireInfo Info = FFireInfo(25000, GetActorLocation(), ETeamInfo::Enemy, false);

	//적중 여부 검색용 맵
	TMap<AActor*, bool> Map;
	
	//컴포넌트 중에서
	for (const auto Component : OutComponents)
	{
		//데미지 컴포넌트를 찾는데 성공했다면
		if (const auto OtherDamageComp = Cast<UDamageComponent>(Component))
		{
			//이미 데미지를 적용한 대상이라면 건너뛰기
			if (Map.Find(OtherDamageComp->GetOwner()) != nullptr)
			{
				continue;
			}

			//몸체가 아니라면 무시한다
			if (OtherDamageComp->BodyType != EBodyType::Body)
			{
				continue;
			}
			
			//데미지 적용
			OtherDamageComp->OnDamaged(Info);

			//소유자 등록
			Map.Add(OtherDamageComp->GetOwner());
		}
	}
}