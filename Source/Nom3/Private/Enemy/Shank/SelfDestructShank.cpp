// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Shank/SelfDestructShank.h"


// Sets default values
ASelfDestructShank::ASelfDestructShank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASelfDestructShank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelfDestructShank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

