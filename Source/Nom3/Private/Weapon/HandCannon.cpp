// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HandCannon.h"


#include "Nom3/Nom3.h"
#include "Weapon/WeaponData.h"


// Sets default values
AHandCannon::AHandCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Pistol/Meshes/SKM_Pistol.SKM_Pistol'"));
	if (TempMesh.Succeeded())
	{
		WeaponMeshComp->SetSkeletalMesh(TempMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataAsset> TempData(TEXT("/Script/Nom3.WeaponData'/Game/Data/HandCannon.HandCannon'"));
	if (TempData.Succeeded())
	{
		UDataAsset* dataAsset = TempData.Object;
		WeaponData = Cast<UWeaponData>(dataAsset);
		CurrentAmmo = WeaponData->AmmoCount;
		MaxAmmo = WeaponData->MaxAmmo;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/gun/HandGunShot_Montage.HandGunShot_Montage'"));
	if (ShotMontage.Succeeded())
		GunShotMontage = ShotMontage.Object;
}

// Called when the game starts or when spawned
void AHandCannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHandCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandCannon::NoAimFire()
{
	Super::NoAimFire();
}

void AHandCannon::AimFire()
{
	Super::AimFire();
}
