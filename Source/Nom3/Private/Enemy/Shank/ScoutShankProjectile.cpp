// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/ScoutShankProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AScoutShankProjectile::AScoutShankProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스태틱 메시 컴포넌트 부착
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
	
	//발사체 속도 설정
	ProjectileMoveComp->InitialSpeed = 2000;
	
	//총탄이 속도를 따라 회전하도록 설정
	ProjectileMoveComp->bRotationFollowsVelocity = true;

	//발사체 정보 초기화
	ProjectileInfo = FProjectileInfo(1500, ProjectileMoveComp->InitialSpeed);
}

void AScoutShankProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (auto Temp = Cast<APawn>(OtherActor) == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Pawn Hit!"));
	}

	//파괴
	this->Destroy();
}