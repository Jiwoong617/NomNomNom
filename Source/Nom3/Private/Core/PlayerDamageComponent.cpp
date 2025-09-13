// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerDamageComponent.h"

#include "Core/NomPlayer.h"


// Sets default values for this component's properties
UPlayerDamageComponent::UPlayerDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPlayerDamageComponent::OnHitBody(FFireInfo& info)
{
	Super::OnHitBody(info);
}

void UPlayerDamageComponent::OnHitHead(FFireInfo& info)
{
	Super::OnHitHead(info);
}


void UPlayerDamageComponent::Init(FVector boxSize, AActor* owner, ECollisionChannel channel, FName collisionPresetName,
	EBodyType bodyType)
{
	Super::Init(boxSize, owner, channel, collisionPresetName, bodyType);

	Owner = Cast<ANomPlayer>(owner);
}

