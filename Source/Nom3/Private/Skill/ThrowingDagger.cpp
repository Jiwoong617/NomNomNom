// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ThrowingDagger.h"

#include "Components/SphereComponent.h"
#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AThrowingDagger::AThrowingDagger()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("Projectile"), true);
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetSphereRadius(12.f);
	SetRootComponent(SphereComp);

	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempmesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Dagger/StaticMeshes/pisau_belati_2_dagger_2.pisau_belati_2_dagger_2'"));
	if (tempmesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempmesh.Object);
		MeshComp->SetRelativeLocation(FVector(-10, 0, -10));
		MeshComp->SetRelativeRotation(FRotator(0, 180, 0));
		MeshComp->SetRelativeScale3D(FVector(0.005f));;
	}

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMoveComp"));
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;
	ProjectileMoveComp->bRotationFollowsVelocity = true;

	Trail = CreateDefaultSubobject<UNiagaraComponent>(FName("Niagara"));
	Trail->SetupAttachment(MeshComp);
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> temptrail(TEXT("/Script/Niagara.NiagaraSystem'/Game/Effect/ArrowTrail/FX/NS_ArrowTrail_Fire.NS_ArrowTrail_Fire'"));
	if (temptrail.Succeeded())
		Trail->SetAsset(temptrail.Object);
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
