// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "Interfaces/Damagable.h"
#include "Kismet/GameplayStatics.h"
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
	CollisionComponent->SetCollisionProfileName(FName("Projectile"), true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetSimulatePhysics(true);
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
	
	ConstructorHelpers::FObjectFinder<USoundWave> tmpSound(TEXT("/Script/Engine.SoundWave'/Game/Asset/Sound/Grenade_Explosion.Grenade_Explosion'"));
	if (tmpSound.Succeeded())
		Sound = tmpSound.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> ns(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/FX/NS_Explode.NS_Explode'"));
	if (ns.Succeeded())
		ExplodeEff = ns.Object;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlap);
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrenade::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ANomPlayer>(OtherActor) == Player)
		return;

	OnExplode();
}

void AGrenade::OnExplode()
{
	EObjectTypeQuery ObjType1 = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	EObjectTypeQuery ObjType2 = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<UPrimitiveComponent*> OutActors;
	TArray<AActor*> ActorsToIgnore;

	TMap<AActor*, bool> Map;
	if (UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetActorLocation(), ExplodeRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType1, ObjType2 }, nullptr, ActorsToIgnore, OutActors))
	{
		for (UPrimitiveComponent* comp : OutActors)
		{
			if (UDamageComponent* act = Cast<UDamageComponent>(comp))
			{
				if (Map.Find(act->GetOwner()) != nullptr)
					continue;
				
				float alpha = FVector::Dist(GetActorLocation(), comp->GetComponentLocation()) / ExplodeRadius;
				float damage = FMath::Clamp(FMath::Lerp(MaxDamage, MinDamage, alpha), MinDamage, MaxDamage);
				act->OnDamaged(FFireInfo(damage, GetActorLocation(), ETeamInfo::Player, false));

				Map.Add(act->GetOwner());
			}
		}
	}

	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeEff,GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplodeRadius, 12, FColor::Red, false, 2.f, 0, 10);
	
	PRINTINFO();
	Destroy();
}

void AGrenade::Init(FVector Direction, float Speed, ANomPlayer* NomPlayer)
{
	Player = NomPlayer;
	CollisionComponent->AddImpulse(Direction * Speed * CollisionComponent->GetMass());
}
