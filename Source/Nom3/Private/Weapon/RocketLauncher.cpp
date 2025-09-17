// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RocketLauncher.h"

#include "Weapon/HomingMissile.h"
#include "Weapon/WeaponData.h"


// Sets default values
ARocketLauncher::ARocketLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/GrenadeLauncher/Meshes/SKM_GrenadeLauncher.SKM_GrenadeLauncher'"));
	if (TempMesh.Succeeded())
	{
		WeaponMeshComp->SetSkeletalMesh(TempMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataAsset> TempData(TEXT("/Script/Nom3.WeaponData'/Game/Data/RocketLauncher.RocketLauncher'"));
	if (TempData.Succeeded())
	{
		UDataAsset* dataAsset = TempData.Object;
		WeaponData = Cast<UWeaponData>(dataAsset);
		CurrentAmmo = WeaponData->AmmoCount;
		MaxAmmo = WeaponData->MaxAmmo;
	}

	ConstructorHelpers::FClassFinder<AHomingMissile> TempMissile(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Weapons/BP_HomingMissile.BP_HomingMissile_C'"));
	if (TempMissile.Succeeded())
		Missile = TempMissile.Class;
}

// Called when the game starts or when spawned
void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocketLauncher::AimFire()
{
	NoAimFire();
}

void ARocketLauncher::NoAimFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, [this]()
	{
		AHomingMissile* mis = GetWorld()->SpawnActor<AHomingMissile>(Missile);
		FTransform trans = WeaponMeshComp->GetSocketTransform(FireSocketName);
		mis->InitFire(WeaponData->Damage, trans.GetLocation(), trans.GetRotation().Rotator());
		
		FireCounter++;
		if (FireCounter == 6)
		{
			GetWorldTimerManager().ClearTimer(FireTimer);
			FireCounter = 0;
		}
	}, 0.1f, true);
}
