// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/DamageComponent.h"

#include "Core/NomPlayer.h"
#include "Enemy/Core/EnemyBase.h"
#include "Enemy/Core/ProjectileBase.h"
#include "Interfaces/Damagable.h"

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	SetGenerateOverlapEvents(true);
}

// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	//이 충돌 컴포넌트만의 충돌 처리 메서드 바인딩
	OnComponentBeginOverlap.AddDynamic(this, &UDamageComponent::OnBeginOverlap);

	//Enemy의 인터페이스 메서드 바인딩
	if (const auto Enemy = Cast<AEnemyBase>(GetOwner()))
	{
		OnDamagedDelegate.AddDynamic(Enemy, &AEnemyBase::OnDamaged);
	}

	//Player의 인터페이스 메서드 바인딩
	if (const auto Player = Cast<ANomPlayer>(GetOwner()))
	{
		OnDamagedDelegate.AddDynamic(Player, &ANomPlayer::OnDamaged);
	}
}

// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageComponent::OnHitBody(FFireInfo& info)
{
	
}

void UDamageComponent::OnHitHead(FFireInfo& info)
{
	
}

void UDamageComponent::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//충돌 대상이 발사체 기반 액터
	if (const auto HitProjectile = Cast<AProjectileBase>(OtherActor))
	{
		//발사체 정보
		FFireInfo Info = HitProjectile->FireInfo;
		
		//데미지 처리을 위하여 호출
		if (BodyType == EBodyType::Body)
		{
			OnHitBody(Info);
		}
		else if (BodyType == EBodyType::Head)
		{
			OnHitHead(Info);
		}

		//정보를 전달한 충돌 발사체는 파괴
		HitProjectile->Destroy();
	}
}

void UDamageComponent::Init(FVector boxSize, ECollisionChannel channel, FName collisionPresetName, EBodyType bodyType)
{
	BoxExtent = boxSize;
	SetCollisionObjectType(channel);
	SetCollisionProfileName(collisionPresetName);
	BodyType = bodyType;
}

void UDamageComponent::Init(ECollisionChannel channel, FName collisionPresetName, EBodyType bodyType)
{
	//박스 범위 지정 없이 초기화
	SetCollisionObjectType(channel);
	SetCollisionProfileName(collisionPresetName);
	BodyType = bodyType;
}

void UDamageComponent::OnDamaged(FFireInfo info)
{
}

void UDamageComponent::Inactive()
{
	//완전히 비활성화
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
	SetCollisionProfileName(FName("NoCollision"));
}