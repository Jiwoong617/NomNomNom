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
	SphereComp->SetGenerateOverlapEvents(true);
	SetRootComponent(SphereComp);

	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempmesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempmesh.Succeeded())
		MeshComp->SetStaticMesh(tempmesh.Object);

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMoveComp"));
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AThrowingDagger::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AThrowingDagger::OnOverlap);
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
