// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponData.h"


// Sets default values
AWeaponBase::AWeaponBase() : WeaponData(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMeshComp;
	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Fire()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		FVector Pos = WeaponOwner->GetFpsCam()->GetComponentLocation();
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, Pos,
			Pos + WeaponOwner->GetFpsCam()->GetForwardVector() * 10000, ECC_Visibility);

		DrawDebugLine(GetWorld(), Pos,
			Pos + WeaponOwner->GetFpsCam()->GetForwardVector() * 10000, FColor::Red, false, 1);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%d/%d - %d"), CurrentAmmo, WeaponData->AmmoCount, MaxAmmo));
	}
}

void AWeaponBase::AimFire()
{
	PRINTINFO();
}

void AWeaponBase::Reload()
{
	int32 NeededAmmo = FMath::Min(MaxAmmo, WeaponData->AmmoCount - CurrentAmmo);
	CurrentAmmo += NeededAmmo;
	MaxAmmo -= NeededAmmo;
}

void AWeaponBase::SetOwner(ANomPlayer* NewOwner)
{
	WeaponOwner = NewOwner;
}

const UWeaponData* AWeaponBase::GetData() const
{
	return WeaponData;
}

FTransform AWeaponBase::GetSocketTransform(FName& SocketName)
{
	return WeaponMeshComp->GetSocketTransform(SocketName);
}
