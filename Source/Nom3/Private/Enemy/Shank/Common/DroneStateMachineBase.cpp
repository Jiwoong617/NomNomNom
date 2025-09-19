// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/DroneStateMachineBase.h"
#include "Enemy/Shank/Common/DroneBase.h"

UDroneStateMachineBase::UDroneStateMachineBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDroneStateMachineBase::BeginPlay()
{
	Super::BeginPlay();

	//유효한 생크에 부착됐다면
	if (auto Temp = Cast<ADroneBase>(GetOwner()))
	{
		OwnerDrone = Temp;
	}
}