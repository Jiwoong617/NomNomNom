// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HomingMissile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemy/Core/EnemyBase.h"
#include "Core/DamageComponent.h"


// Sets default values
AHomingMissile::AHomingMissile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SphereComp->SetGenerateOverlapEvents(true);

	ProjectileMoveComp->MaxSpeed = Speed;
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;
	ProjectileMoveComp->bIsHomingProjectile = false;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHomingMissile::OnOverlap);

	FTimerHandle HomingTimer;
	GetWorldTimerManager().SetTimer(HomingTimer,this, &AHomingMissile::SetHoming, HomingDelay, false);
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHomingMissile::SetHoming()
{
	AActor* ClosestEnemy = nullptr;
	float ClosestSq = TNumericLimits<float>::Max();

	TArray<AActor*> Candidates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), Candidates);
	const FVector MyLoc = GetActorLocation();
	for (AActor* Actor : Candidates)
	{
		if (!IsValid(Actor)) continue;
		const AEnemyBase* Enemy = Cast<AEnemyBase>(Actor);
		if (!Enemy) continue;
		if (Enemy->GetHP() <= 0) continue;

		const float DistSq = FVector::DistSquared(MyLoc, Actor->GetActorLocation());
		if (DistSq < ClosestSq)
		{
			ClosestSq = DistSq;
			ClosestEnemy = Actor;
		}
	}

	if (ClosestEnemy)
	{
		if (UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(ClosestEnemy->GetRootComponent()))
		{
			ProjectileMoveComp->HomingTargetComponent = TargetComp;
			ProjectileMoveComp->HomingAccelerationMagnitude = HomingStrength;
			ProjectileMoveComp->bIsHomingProjectile = true;
			return;
		}
	}

	//적 없으면
	ProjectileMoveComp->bIsHomingProjectile = false;
	FTimerHandle ExplodeTimer;
	GetWorldTimerManager().SetTimer(ExplodeTimer, this, &AHomingMissile::Explode, StraightFlightExplodeDelay, false);
}

void AHomingMissile::Explode()
{
	EObjectTypeQuery ObjType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<UPrimitiveComponent*> OutComponents;
	TArray<AActor*> ActorsToIgnore;

	if (UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetActorLocation(), ExplosionRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>{ ObjType }, nullptr, ActorsToIgnore, OutComponents))
	{
		for (UPrimitiveComponent* Comp : OutComponents)
		{
			if (UDamageComponent* DamageComp = Cast<UDamageComponent>(Comp))
			{
				float Alpha = FVector::Dist(GetActorLocation(), Comp->GetComponentLocation()) / ExplosionRadius;
				float Damage = FMath::Lerp(FireInfo.Damage, FireInfo.Damage/2, Alpha);
				DamageComp->OnDamaged(FFireInfo(Damage, GetActorLocation(), ETeamInfo::Player, false));
			}
		}
	}

	Destroy();
}

void AHomingMissile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}

void AHomingMissile::InitFire(int32 Dmg, FVector Loc, FRotator Rot)
{
	FireInfo.Damage = Dmg;
	FireInfo.ProjectileSpeed = Speed;
	FireInfo.bIsIgnoreTeam = false;
	FireInfo.TeamInfo = ETeamInfo::Player;
	FireInfo.FireLocation = Loc;
	
	SetActorLocationAndRotation(Loc, Rot);

	//이동 방향 전환
	if (ProjectileMoveComp)
		ProjectileMoveComp->Velocity = FireInfo.ProjectileSpeed * GetActorForwardVector();
}
