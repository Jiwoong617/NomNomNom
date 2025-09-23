// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HomingMissile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "Enemy/Core/EnemyActorBase.h"
#include "Enemy/Core/EnemyHealthComponent.h"


// Sets default values
AHomingMissile::AHomingMissile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("Projectile"), true);
	SetRootComponent(SphereComp);

	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SphereComp->SetGenerateOverlapEvents(true);

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMoveComp"));
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
	GetWorldTimerManager().SetTimer(CantReachTimer,[this]()
	{
		Explode();
	}, 5.f, false);
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHomingMissile::SetHoming()
{
	TArray<AActor*> EnemyList;
	
	TArray<AActor*> Candidates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActorBase::StaticClass(), Candidates);
	for (AActor* Actor : Candidates)
	{
		if (!IsValid(Actor)) continue;
		AEnemyActorBase* Enemy = Cast<AEnemyActorBase>(Actor);
		if (!Enemy) continue;
		if (Enemy->GetComponentByClass<UEnemyHealthComponent>()->GetHP() <= 0)
			continue;

		if (FVector::Dist(GetActorLocation(), Actor->GetActorLocation()) < 15000)
			EnemyList.Add(Enemy);
	}


	if (EnemyList.Num() > 0)
	{
		AActor* randEnemy= EnemyList[FMath::RandRange(0, EnemyList.Num() - 1)];
			
		if (UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(randEnemy->GetRootComponent()))
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
	EObjectTypeQuery ObjType1 = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	EObjectTypeQuery ObjType2 = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<UPrimitiveComponent*> OutComponents;
	TArray<AActor*> ActorsToIgnore = {Player};
	
	TMap<AActor*, bool> Map;
	if (UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetActorLocation(), ExplosionRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>{ ObjType1, ObjType2 }, nullptr, ActorsToIgnore, OutComponents))
	{
		for (UPrimitiveComponent* Comp : OutComponents)
		{
			if (UDamageComponent* DamageComp = Cast<UDamageComponent>(Comp))
			{
				if (Map.Find(DamageComp->GetOwner()) != nullptr)
					continue;

				if (DamageComp->GetOwner() == Player)
					continue;
				
				float Alpha = FVector::Dist(GetActorLocation(), Comp->GetComponentLocation()) / ExplosionRadius;
				float dmg = FMath::Clamp(FMath::Lerp(Damage, Damage/2, Alpha), Damage/2, Damage);
				DamageComp->OnDamaged(FFireInfo(dmg, GetActorLocation(), ETeamInfo::Player, false));
				
				Map.Add(DamageComp->GetOwner());
			}
		}
	}

	//TODO : 이펙트 넣기
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.f, 0, 10);

	GetWorldTimerManager().ClearTimer(CantReachTimer);
	Destroy();
}

void AHomingMissile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}

void AHomingMissile::InitFire(int32 Dmg, ANomPlayer* NomPlayer)
{
	Damage = Dmg;
	Player = NomPlayer;
	ProjectileMoveComp->Velocity = Speed * GetActorForwardVector();
}
