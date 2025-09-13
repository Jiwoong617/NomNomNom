// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade.h"

#include "Components/SphereComponent.h"
#include "Interfaces/Damagable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Nom3/Nom3.h"


// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(20.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetLinearDamping(0.01f);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->SetupAttachment(CollisionComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Grenade/GrenadeMesh.GrenadeMesh'"));
	if (TempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(TempMesh.Object);
		Mesh->SetRelativeScale3D(FVector(50));
		Mesh->SetRelativeLocation(FVector(0, -7, 0));
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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
	if (IsHit) return;
	CollisionComponent->SetLinearDamping(2.f);
	
	if (IDamagable* enemy = Cast<IDamagable>(OtherActor))
	{
		OnExplode();
		return;
	}

	GetWorldTimerManager().SetTimer(TimerHandle,this, &AGrenade::OnExplode, 2.f, false);
	
	IsHit = true;
}

void AGrenade::OnExplode()
{
	EObjectTypeQuery ObjType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore;
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplodeRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType}, nullptr, ActorsToIgnore, OutActors))
	{
		for (AActor* actor : OutActors)
		{
			if (IDamagable* act = Cast<IDamagable>(actor))
			{
				float Damage = FVector::Dist(GetActorLocation(), actor->GetActorLocation()) / ExplodeRadius;
				//act->OnDamaged(FMath::Lerp(MaxDamage, MinDamage, Damage));
			}
		}
	}

	
	//TODO : 이펙트 넣기
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplodeRadius, 12, FColor::Red, false, 2.f, 0, 10);
	
	PRINTINFO();
	Destroy();
}

void AGrenade::Init(FVector Direction, float Speed)
{
	CollisionComponent->AddImpulse(Direction * Speed * CollisionComponent->GetMass());
}
