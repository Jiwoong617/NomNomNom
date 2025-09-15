// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"


#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

		OnBulletChangeDelegate.Broadcast(CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
	}
    
    CamOffset = Owner->GetFpsCamArm()->GetRelativeLocation();
    // 현재 카메라 FOV를 저장해 원래 값으로 정확히 복귀
    CameraFOV = Owner->GetFpsCam()->FieldOfView;
	//에임 소켓 위치 월드 -> 로컬로 변환
	const FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
	const USceneComponent* ParentComp = Owner->GetFpsCamArm()->GetAttachParent();
	const FName ParentSocket = Owner->GetFpsCamArm()->GetAttachSocketName();
	FTransform ParentFrame = ParentComp ? ParentComp->GetComponentTransform() : FTransform::Identity;
	if (const auto SkelParent = Cast<USkeletalMeshComponent>(ParentComp))
	{
		if (ParentSocket != NAME_None)
		{
			ParentFrame = SkelParent->GetSocketTransform(ParentSocket, RTS_World);
		}
	}
	const FVector AimSocketLocLocal = ParentFrame.InverseTransformPosition(AimSocketLoc);
	AimCamLoc = AimSocketLocLocal;

	OnChangeWeaponDelegate.Broadcast(0, CurrentWeapon->GetData()->WeaponImg, CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
	OnChangeWeaponDelegate.Broadcast(1, WeaponList[1]->GetData()->WeaponImg, WeaponList[1]->CurrentAmmo, 0);
	OnChangeWeaponDelegate.Broadcast(2, WeaponList[2]->GetData()->WeaponImg, WeaponList[2]->CurrentAmmo, 0);
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

		OnBulletChangeDelegate.Broadcast(CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
	}
}

void UWeaponComponent::Reload()
{
	CurrentWeapon->Reload();

	OnBulletChangeDelegate.Broadcast(CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
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
    const float Delta = GetWorld()->GetDeltaSeconds();
    const float Duration = CurrentWeapon ? CurrentWeapon->GetData()->AimDuration : 0.1f;

    if (CurrentWeapon && Owner && CurrentWeapon->GetData()->IsAimable)
    {
        const FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
        const USceneComponent* ParentComp = Owner->GetFpsCamArm()->GetAttachParent();
        const FName ParentSocket = Owner->GetFpsCamArm()->GetAttachSocketName();
        FTransform ParentFrame = ParentComp ? ParentComp->GetComponentTransform() : FTransform::Identity;
        if (const auto SkelParent = Cast<USkeletalMeshComponent>(ParentComp))
        {
            if (ParentSocket != NAME_None)
            {
                ParentFrame = SkelParent->GetSocketTransform(ParentSocket, RTS_World);
            }
        }
        const FVector AimSocketLocLocal = ParentFrame.InverseTransformPosition(AimSocketLoc);
        AimCamLoc = AimSocketLocLocal;
    }

    if (bIsAiming)
    {
        AimTime = FMath::Min(AimTime + Delta, Duration);
        const float Alpha = FMath::Clamp(AimTime / Duration, 0.f, 1.f);
        Owner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(CamOffset, AimCamLoc, Alpha));
        Owner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CameraFOV, CurrentWeapon->GetData()->ADS_FOV, Alpha));
    }
    else
    {
        AimTime = FMath::Max(AimTime - Delta, 0.f);
        const float Alpha = FMath::Clamp(1.f - (AimTime / Duration), 0.f, 1.f);
        Owner->GetFpsCamArm()->SetRelativeLocation(FMath::Lerp(AimCamLoc, CamOffset, Alpha));
        Owner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CurrentWeapon->GetData()->ADS_FOV, CameraFOV, Alpha));
    }
}

void UWeaponComponent::ChangeWeapon(int32 idx)
{
	if (idx == CurrentWeaponIdx)
	{
		return;
	}
	
	OnChangeWeaponDelegate.Broadcast((idx == 0 ? CurrentWeaponIdx : idx), CurrentWeapon->GetData()->WeaponImg, CurrentWeapon->CurrentAmmo, 0);
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeaponIdx = idx;
	CurrentWeapon = WeaponList[idx];
	WeaponList[idx]->SetActorHiddenInGame(false);
	OnChangeWeaponDelegate.Broadcast(0, CurrentWeapon->GetData()->WeaponImg, CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
}

void UWeaponComponent::OnWeaponChanged(int32 idx)
{
	if (CurrentWeapon->GetData()->IsAimable == false)
		return;
	const FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
	const USceneComponent* ParentComp = Owner->GetFpsCamArm()->GetAttachParent();
	const FName ParentSocket = Owner->GetFpsCamArm()->GetAttachSocketName();
	FTransform ParentFrame = ParentComp ? ParentComp->GetComponentTransform() : FTransform::Identity;
	if (const auto SkelParent = Cast<USkeletalMeshComponent>(ParentComp))
	{
		if (ParentSocket != NAME_None)
		{
			ParentFrame = SkelParent->GetSocketTransform(ParentSocket, RTS_World);
		}
	}
	const FVector AimSocketLocLocal = ParentFrame.InverseTransformPosition(AimSocketLoc);
	//AimCamLoc = FVector(CamOffset.X, AimSocketLocLocal.Y, AimSocketLocLocal.Z);		
	AimCamLoc = AimSocketLocLocal;

	//OnBulletChangeDelegate.Broadcast(CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
}

AWeaponBase* UWeaponComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 UWeaponComponent::GetCurrentWeaponIdx()
{
	return CurrentWeaponIdx;
}
