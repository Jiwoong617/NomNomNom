// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shank/Common/ShankStateMachineBase.h"
#include "Enemy/Shank/Common/ShankBase.h"

UShankStateMachineBase::UShankStateMachineBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UShankStateMachineBase::BeginPlay()
{
	Super::BeginPlay();

	//유효한 생크에 부착됐다면
	if (auto Temp = Cast<AShankBase>(GetOwner()))
	{
		OwnerShank = Temp;
	}
}