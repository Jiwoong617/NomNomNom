// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DamageComponent.h"
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

	// ...
	
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
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

void UDamageComponent::OnDamaged(FFireInfo FireInfo)
{
	if (BodyType == EBodyType::Body)
		OnHitBody(FireInfo);
	else
		OnHitHead(FireInfo);
}

void UDamageComponent::Init(FVector boxSize, AActor* owner, ECollisionChannel channel, FName collisionPresetName,
                            EBodyType bodyType)
{
	BoxExtent = boxSize;
	SetCollisionObjectType(channel);
	SetCollisionProfileName(collisionPresetName);
	BodyType = bodyType;
}

