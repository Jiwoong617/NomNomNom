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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/ani/handCanonShot_Anim_Montage.handCanonShot_Anim_Montage'"));
	if (ShotMontage.Succeeded())
		GunShotMontage = ShotMontage.Object;

	//발사 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
	TEXT("/Game/Asset/Weapon/Sound/SC_RevolverGunFire.SC_RevolverGunFire"));
	Finder.Succeeded())
	{
		FireSound = Finder.Object;
	}

	//탄피 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/Weapon/Sound/SC_ShellHittingGround.SC_ShellHittingGround"));
		Finder.Succeeded())
	{
		ShellSound = Finder.Object;
	}

	//재장전 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/Weapon/Sound/SC_RevolverGunReload.SC_RevolverGunReload"));
		Finder.Succeeded())
	{
		ReloadSound = Finder.Object;
	}
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
