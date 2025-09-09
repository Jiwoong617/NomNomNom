// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"


#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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

	if (CurrentWeapon->IsHidden() == false)
	{
		Fire();
		OnAiming();
	}
}

void UWeaponComponent::Init()
{
	Owner = Cast<ANomPlayer>(GetOwner());

	TArray<UChildActorComponent*> ChildComps;
	GetOwner()->GetComponents<UChildActorComponent>(ChildComps);
	for (UChildActorComponent* child : ChildComps)
	{
		if (AWeaponBase* wb = Cast<AWeaponBase>(child->GetChildActor()))
		{
			wb->SetOwner(Owner);
			wb->SetActorHiddenInGame(true);
			WeaponList.Add(wb);
		}
	}
	
	//PRINTLOG(TEXT("%d"), WeaponList.Num());
	
	if (WeaponList.Num() > 0)
	{
		WeaponList.Sort([](const AWeaponBase& A, const AWeaponBase& B)
		{
			return A.GetData()->WeaponType < B.GetData()->WeaponType;
		});
		CurrentWeaponIdx = 0;
		CurrentWeapon = WeaponList[CurrentWeaponIdx];
		CurrentWeapon->SetActorHiddenInGame(false);
	}
	
	CamOffset = Owner->GetFpsCamArm()->GetRelativeLocation();
	//에임 소켓 위치 월드 -> 로컬로 변환
	FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
	FVector AimSocketLocLocal = Owner->GetFpsCamArm()->GetAttachParent()->
		GetComponentTransform().InverseTransformPosition(AimSocketLoc);
	
	AimCamLoc = FVector(CamOffset.X, AimSocketLocLocal.Y, AimSocketLocLocal.Z);
}

void UWeaponComponent::FireStart()
{
	bIsFiring = true;
}

void UWeaponComponent::FireEnd()
{
	bIsFiring = false;
}

void UWeaponComponent::Fire()
{
	FireTime += GetWorld()->GetDeltaSeconds();
	
	if (FireTime > CurrentWeapon->GetData()->FireRate && bIsFiring)
	{
		FireTime = 0.0f;
		if (bIsAiming)
			CurrentWeapon->AimFire();
		else
			CurrentWeapon->NoAimFire();
	}
}

void UWeaponComponent::Reload()
{
	CurrentWeapon->Reload();
}

void UWeaponComponent::AimStart()
{
	if (CurrentWeapon->GetData()->IsAimable == false)
	{
		return;
	}
	
	bIsAiming = true;
}

void UWeaponComponent::AimEnd()
{
	bIsAiming = false;
}

void UWeaponComponent::OnAiming()
{
	if (bIsAiming)
	{
		if (AimTime >= CurrentWeapon->GetData()->AimDuration) return;
		AimTime += GetWorld()->GetDeltaSeconds();
		
		Owner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(CamOffset, AimCamLoc, AimTime / CurrentWeapon->GetData()->AimDuration));
		Owner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CameraFOV, CurrentWeapon->GetData()->ADS_FOV, AimTime / CurrentWeapon->GetData()->AimDuration));
	}
	else
	{
		if (AimTime <= 0) return;
		AimTime -= GetWorld()->GetDeltaSeconds();
		
		Owner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(AimCamLoc, CamOffset, 1.f - AimTime / CurrentWeapon->GetData()->AimDuration));
		Owner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CurrentWeapon->GetData()->ADS_FOV, CameraFOV, 1.f - AimTime / CurrentWeapon->GetData()->AimDuration));
	}
}

void UWeaponComponent::ChangeWeapon(int32 idx)
{
	if (idx == CurrentWeaponIdx)
	{
		return;
	}
	
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeaponIdx = idx;
	CurrentWeapon = WeaponList[idx];
	WeaponList[idx]->SetActorHiddenInGame(false);
}

void UWeaponComponent::OnWeaponChanged(int32 idx)
{
	if (CurrentWeapon->GetData()->IsAimable == false)
		return;
	FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
	FVector AimSocketLocLocal = Owner->GetFpsCamArm()->GetAttachParent()->
		GetComponentTransform().InverseTransformPosition(AimSocketLoc);
	
	AimCamLoc = FVector(CamOffset.X, AimSocketLocLocal.Y, AimSocketLocLocal.Z);
}

AWeaponBase* UWeaponComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 UWeaponComponent::GetCurrentWeaponIdx()
{
	return CurrentWeaponIdx;
}
