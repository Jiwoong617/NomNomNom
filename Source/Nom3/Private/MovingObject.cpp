// Fill out your copyright notice in the Description page of Project Settings.


#include "Nom3/Public/MovingObject.h"


// Sets default values
AMovingObject::AMovingObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();
	// StartLocation = GetOwner()->GetActorLocation();
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 두 포인트를 움직여야 함 , 포인트 두개가 있어야함 , 보간된 속도로 움직여야함 , 포인트에 들어선다면 일정 시간동안 그자리에 멈춰있어야함 ,
	// 
}

