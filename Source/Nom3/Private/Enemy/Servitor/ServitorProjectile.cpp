// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Servitor/ServitorProjectile.h"

#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "Enemy/Core/EnemyCharacterBase.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AServitorProjectile::AServitorProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//스태틱 메시 컴포넌트 부착
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	MeshComp->SetCollisionProfileName(FName("NoCollision"));
	MeshComp->SetupAttachment(GetRootComponent());
	
	//발사체 속도 설정
	ProjectileMoveComp->InitialSpeed = 1500;

	//발사체 중력 설정
	ProjectileMoveComp->ProjectileGravityScale = 0.1f;
	
	//총탄이 속도를 따라 회전하도록 설정
	ProjectileMoveComp->bRotationFollowsVelocity = true;

	//발사체 정보 초기화
	FireInfo = FFireInfo(0, FVector::ZeroVector, ETeamInfo::Enemy, true, ProjectileMoveComp->InitialSpeed);
}

void AServitorProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (auto Temp = Cast<ANomPlayer>(OtherActor))
	{
		Explode();
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Player Hit!"));
		return;
	}

	if (auto Temp = Cast<AEnemyActorBase>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("EnemyActor Hit!"));
		return;	
	}

	if (auto Temp = Cast<AEnemyCharacterBase>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("EnemyCharacter Hit!"));
		return;
	}

	if (auto Temp = Cast<AStaticMeshActor>(OtherActor))
	{
		Explode();
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Other Hit!"));
		return;
	}
}

void AServitorProjectile::Explode()
{
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
	const FFireInfo Info = FFireInfo(15, GetActorLocation(), ETeamInfo::Enemy, false);

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
