// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"

#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Init();
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::Init()
{
	// TODO : 수정해야됨 - ChildActor로
	for (UWeaponData* Data : WeaponDataList)
	{
		if (!Data) continue;

		AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass());
		NewWeapon->InitializeWeapon(Data);

		//NewWeapon->SetActorHiddenInGame(true);
		WeaponList.Add(Data->WeaponType, NewWeapon);
	}

	CurrentWeapon = WeaponList[EWeaponType::HandCannon];
}

void UWeaponComponent::Fire()
{
	CurrentWeapon->Fire();
}

void UWeaponComponent::Reload()
{
	CurrentWeapon->Reload();
}

void UWeaponComponent::Aim()
{
	CurrentWeapon->Aim();
}

void UWeaponComponent::ChangeWeapon(EWeaponType WeaponType)
{
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeapon = WeaponList[WeaponType];
	CurrentWeapon->SetActorHiddenInGame(false);
}