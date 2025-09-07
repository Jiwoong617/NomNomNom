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

	if (IsHidden() == false && CurrentRecoil != TargetRecoil)
	{
		CurrentRecoil = FMath::RInterpTo(CurrentRecoil, TargetRecoil, DeltaTime, 20.f);
		FRotator DeltaRecoil = CurrentRecoil - LastAppliedRecoil;
		WeaponOwner->GetController()->SetControlRotation(WeaponOwner->GetControlRotation() + DeltaRecoil);
		LastAppliedRecoil = CurrentRecoil;
	}
}

void AWeaponBase::ApplyRecoil()
{
	const float RecoilPitch = FMath::RandRange(WeaponData->RecoilPitchMin, WeaponData->RecoilPitchMax);
	const float RecoilYaw = FMath::RandRange(WeaponData->RecoilYawMin, WeaponData->RecoilYawMax) * FMath::RandBool();
	
	TargetRecoil.Pitch += RecoilPitch;
	TargetRecoil.Yaw += RecoilYaw;
	
	//값 제한 -180~180
	TargetRecoil.Pitch = FRotator::NormalizeAxis(TargetRecoil.Pitch);
	TargetRecoil.Yaw = FRotator::NormalizeAxis(TargetRecoil.Yaw);
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

		//Recoil
		ApplyRecoil();
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


float AWeaponBase::EaseElasticOut(float t)
{
	if (t == 0.f) return 0.f;
	if (t == 1.f) return 1.f;

	float p = 0.3f;
	return FMath::Pow(2.f, -10.f * t) * FMath::Sin((t - p / 4.f) * (2 * PI) / p) + 1.f;
}
