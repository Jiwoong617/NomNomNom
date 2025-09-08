// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/SpringArmComponent.h"
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

	CamOffset = WeaponOwner->GetFpsCamArm()->GetRelativeLocation();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHidden() == false)
	{
		Fire();
		ApplyingRecoil();
		OnAiming();
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

void AWeaponBase::ApplyingRecoil()
{
	if (CurrentRecoil != TargetRecoil)
	{
		CurrentRecoil = FMath::RInterpTo(CurrentRecoil, TargetRecoil, GetWorld()->GetDeltaSeconds(), 20.f);
		FRotator DeltaRecoil = CurrentRecoil - LastAppliedRecoil;
		WeaponOwner->GetController()->SetControlRotation(WeaponOwner->GetControlRotation() + DeltaRecoil);
		LastAppliedRecoil = CurrentRecoil;
	}
}

void AWeaponBase::FireStart()
{
	bIsFiring = true;
}

void AWeaponBase::FireEnd()
{
	bIsFiring = false;
}

void AWeaponBase::Fire()
{
	FireTime += GetWorld()->GetDeltaSeconds();
	
	if (FireTime > WeaponData->FireRate && bIsFiring)
	{
		FireTime = 0.0f;
		if (bIsAiming)
			AimFire();
		else
			NoAimFire();
	}
}

void AWeaponBase::AimFire()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		FVector Pos = WeaponOwner->GetFpsCam()->GetComponentLocation();
		FVector Dir = WeaponOwner->GetFpsCam()->GetForwardVector();
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, Pos,Pos + Dir * 10000, ECC_Visibility);

		//DrawDebugLine(GetWorld(), Pos, Dir * 10000, FColor::Red, false, 1);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%d/%d - %d"), CurrentAmmo, WeaponData->AmmoCount, MaxAmmo));

		//Recoil
		ApplyRecoil();
	}
}

void AWeaponBase::NoAimFire()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;

		//탄퍼짐
		float SpreadHalf = WeaponData->BulletSpread * 0.5f;
		float RandPitch = FMath::RandRange(-SpreadHalf, SpreadHalf);
		float RandYaw   = FMath::RandRange(-SpreadHalf, SpreadHalf);

		FVector Pos = WeaponOwner->GetFpsCam()->GetComponentLocation();
		FVector Dir = WeaponOwner->GetFpsCam()->GetForwardVector();
		FRotator SpreadRot = Dir.Rotation();
		SpreadRot.Pitch += RandPitch;
		SpreadRot.Yaw += RandYaw;
		FVector FinalDir = SpreadRot.Vector();

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,Pos,Pos + FinalDir * 10000,ECC_Visibility);
		
		//DrawDebugLine(GetWorld(), Pos, Pos + FinalDir * 10000, FColor::Red, false, 1);
		//DrawDebugSphere(GetWorld(), Hit.Location, 10, 1, FColor::Red, false, 3);
		
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%d/%d - %d"), CurrentAmmo, WeaponData->AmmoCount, MaxAmmo)
		);

		//Recoil
		ApplyRecoil();
	}
}

void AWeaponBase::ReloadStart()
{
	if (CurrentAmmo == WeaponData->AmmoCount)
		return;

	bIsReloading = true;
	//TODO : Player로 옮기기
	// GetWorld()->GetTimerManager().SetTimer(ReloadHandle, [this]()
	// {
	// 	ReloadEnd();
	// }, WeaponData->ReloadDuration, false);
}

void AWeaponBase::ReloadEnd()
{
	Reload();
	bIsReloading = false;
}

void AWeaponBase::Reload()
{
	int32 NeededAmmo = FMath::Min(MaxAmmo, WeaponData->AmmoCount - CurrentAmmo);
	CurrentAmmo += NeededAmmo;
	MaxAmmo -= NeededAmmo;
}

void AWeaponBase::AimStart()
{
	if (WeaponData->IsAimable == false)
	{
		return;
	}
	
	bIsAiming = true;

	//에임 소켓 위치 월드 -> 로컬로 변환
	FVector AimSocketLoc = GetSocketTransform(AimSocket).GetLocation();
	FVector AimSocketLocLocal = WeaponOwner->GetFpsCamArm()->GetAttachParent()->
		GetComponentTransform().InverseTransformPosition(AimSocketLoc);
	
	AimCamLoc = FVector(CamOffset.X, AimSocketLocLocal.Y, AimSocketLocLocal.Z);
	AimTime = 0.f;
}

void AWeaponBase::AimEnd()
{
	bIsAiming = false;
}

void AWeaponBase::OnAiming()
{
	if (bIsAiming)
	{
		if (AimTime >= WeaponData->AimDuration) return;
		AimTime += GetWorld()->GetDeltaSeconds();
		
		WeaponOwner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(CamOffset, AimCamLoc, AimTime/WeaponData->AimDuration));
		WeaponOwner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CameraFOV, WeaponData->ADS_FOV, AimTime/WeaponData->AimDuration));
	}
	else
	{
		if (AimTime <= 0) return;
		AimTime -= GetWorld()->GetDeltaSeconds();
		
		WeaponOwner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(AimCamLoc, CamOffset, 1.f - AimTime/WeaponData->AimDuration));
		WeaponOwner->GetFpsCam()->SetFieldOfView(FMath::Lerp(WeaponData->ADS_FOV, CameraFOV, 1.f - AimTime/WeaponData->AimDuration));
	}
}

void AWeaponBase::OnFireCanceled()
{
}

void AWeaponBase::OnReloadCanceled()
{
}

void AWeaponBase::OnAimCanceled()
{
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
