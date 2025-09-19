// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"


#include "Camera/CameraComponent.h"
#include "Core/NomPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//트리거 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase>
		Finder(TEXT("/Game/Asset/Weapon/Sound/SC_Trigger.SC_Trigger"));
		Finder.Succeeded())
	{
		TriggerSound = Finder.Object;
	}
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
			child->AttachToComponent( Owner->GetMesh(),
				FAttachmentTransformRules(
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::KeepWorld,
				  EAttachmentRule::SnapToTarget,
				  false),TEXT("WeaponSocket"));
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

	// Initialize UI slot mapping (weaponIdx -> slotIdx)
	WeaponIndexToSlot.SetNum(WeaponList.Num());
	for (int32 i = 0; i < WeaponList.Num(); ++i)
		WeaponIndexToSlot[i] = i;

	for (int32 i = 0; i < WeaponList.Num(); ++i)
	{
		int32 slot = WeaponIndexToSlot[i];
		int32 maxAmmo = (i == CurrentWeaponIdx) ? WeaponList[i]->MaxAmmo : 0;
		OnChangeWeaponDelegate.Broadcast(slot, WeaponList[i]->GetData()->WeaponImg, WeaponList[i]->CurrentAmmo, maxAmmo);
	}
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
		//사격 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TriggerSound, GetOwner()->GetActorLocation());
		
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
	FireTime = CurrentWeapon->GetData()->FireRate;

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

	//바뀐 무기 변경
	int32 oldCurrentIdx = CurrentWeaponIdx;
	int32 targetSlot = WeaponIndexToSlot.IsValidIndex(idx) ? WeaponIndexToSlot[idx] : idx;
	OnChangeWeaponDelegate.Broadcast(targetSlot, CurrentWeapon->GetData()->WeaponImg, CurrentWeapon->CurrentAmmo, 0);
	
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeaponIdx = idx;
	CurrentWeapon = WeaponList[idx];
	WeaponList[idx]->SetActorHiddenInGame(false);
	OnChangeWeaponDelegate.Broadcast(0, CurrentWeapon->GetData()->WeaponImg, CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);

	Owner->PlayFPSAnim(CurrentWeapon->ChangeWeaponMontage, EActionState::ChangeWeapon, idx);
	
	// UI 슬롯 매핑
	if (WeaponIndexToSlot.IsValidIndex(oldCurrentIdx) && WeaponIndexToSlot.IsValidIndex(idx))
	{
		int32 tmp = WeaponIndexToSlot[oldCurrentIdx];
		WeaponIndexToSlot[oldCurrentIdx] = WeaponIndexToSlot[idx];
		WeaponIndexToSlot[idx] = tmp;
	}
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
	AimCamLoc = AimSocketLocLocal;

	FireTime = CurrentWeapon->GetData()->FireRate;
}

AWeaponBase* UWeaponComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 UWeaponComponent::GetCurrentWeaponIdx()
{
	return CurrentWeaponIdx;
}
