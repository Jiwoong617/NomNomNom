// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ThrowingDagger.h"

#include "Components/SphereComponent.h"
#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AThrowingDagger::AThrowingDagger()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("Projectile"), true);
	SetRootComponent(SphereComp);

	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SphereComp->SetGenerateOverlapEvents(true);

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMoveComp"));
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AThrowingDagger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowingDagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowingDagger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ANomPlayer>(OtherActor) == Player)
		return;
	
	if (UDamageComponent* Dmg = Cast<UDamageComponent>(OtherComp))
		Dmg->OnDamaged(FireInfo);

	Destroy();
}

void AThrowingDagger::InitFire(int32 Dmg, int32 Speed, AActor* player)
{
	FireInfo  = FFireInfo(Dmg, player->GetActorLocation(), ETeamInfo::Player, false, Speed);
	ProjectileMoveComp->MaxSpeed = Speed;
	Player = player;
	ProjectileMoveComp->Velocity = Speed * GetActorForwardVector();
}
