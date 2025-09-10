// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(50.f);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(TempMesh.Object);
	}
	Mesh->SetupAttachment(CollisionComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bShouldBounce = true;
	Projectile->Bounciness = 0.3f;
	Projectile->Friction = 0.5f;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
}

void AGrenade::Init(FVector Direction, float Speed)
{
	Projectile->MaxSpeed = Speed;
	Projectile->Velocity = Direction * Speed;
}
