// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"

#include <gsl/pointers>

#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/NomPlayer.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponData.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	ConstructorHelpers::FObjectFinder<UCurveFloat> TempCurve(TEXT("/Script/Engine.CurveFloat'/Game/Data/AimCurve.AimCurve'"));
	if (TempCurve.Succeeded())
	{
		AimCurve = TempCurve.Object;
	}
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
	Fire();
	AimTimeline.TickTimeline(DeltaTime);
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
		FireTime = CurrentWeapon->GetData()->FireRate;
	}

	//Aiming
	if (AimCurve)
	{
		CamOffset = Owner->GetFpsCamArm()->GetRelativeLocation();
		
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("OnAiming"));
		AimTimeline.AddInterpFloat(AimCurve, Progress);
		AimTimeline.SetLooping(false);
	}
}

void UWeaponComponent::FireStart()
{
	if (WeaponState == EWeaponState::Idle)
	{
		WeaponState = EWeaponState::Fire;
		bIsFiring = true;
	}
}

void UWeaponComponent::FireEnd()
{
	WeaponState = EWeaponState::Idle;
	bIsFiring = false;
}

void UWeaponComponent::Fire()
{
	FireTime += GetWorld()->GetDeltaSeconds();
	
	if (FireTime > CurrentWeapon->GetData()->FireRate && WeaponState == EWeaponState::Fire)
	{
		FireTime = 0.0f;
		if (bIsAim)
		{
			CurrentWeapon->AimFire();
		}
		else
		{
			CurrentWeapon->Fire();
		}
	}
}

void UWeaponComponent::ReloadStart()
{
	if (CurrentWeapon->CurrentAmmo == CurrentWeapon->GetData()->AmmoCount)
		return;
	
	if (WeaponState == EWeaponState::Idle || WeaponState == EWeaponState::Fire)
	{
		WeaponState = EWeaponState::Reload;

		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, [this]()
		{
			ReloadEnd();
		}, CurrentWeapon->GetData()->ReloadDuration, false);
	}
}

void UWeaponComponent::ReloadEnd()
{
	CurrentWeapon->Reload();
	WeaponState = bIsFiring ? EWeaponState::Fire : EWeaponState::Idle;
}

void UWeaponComponent::AimStart()
{
	if (CurrentWeapon->GetData()->IsAimable == false)
	{
		return;
	}

	bIsAim = true;

	//에임 소켓 위치 월드 -> 로컬로 변환
	FVector AimSocketLoc = CurrentWeapon->GetSocketTransform(AimSocket).GetLocation();
	FVector AimSocketLocLocal = Owner->GetFpsCamArm()->GetAttachParent()->
		GetComponentTransform().InverseTransformPosition(AimSocketLoc);
	
	AimCamLoc = FVector(CamOffset.X, AimSocketLocLocal.Y, AimSocketLocLocal.Z);
	
	AimTimeline.PlayFromStart();
}

void UWeaponComponent::AimEnd()
{
	bIsAim = false;
	AimTimeline.Reverse();
}

void UWeaponComponent::OnAiming(float Value)
{
	FVector NewLoc(
		CamOffset.X,
		FMath::Lerp(CamOffset.Y, AimCamLoc.Y, Value),
		FMath::Lerp(CamOffset.Z, AimCamLoc.Z, Value)
	);
	Owner->GetFpsCamArm()->SetRelativeLocation(NewLoc);
	
	Owner->GetFpsCam()->SetFieldOfView(FMath::Lerp(CameraFOV, CurrentWeapon->GetData()->ADS_FOV, Value));
}

void UWeaponComponent::ChangeWeapon(int32 idx)
{
	if (idx == CurrentWeaponIdx)
	{
		return;
	}

	WeaponState = EWeaponState::Changing;
	GetWorld()->GetTimerManager().ClearTimer(ChangeHandle);
	GetWorld()->GetTimerManager().SetTimer(ChangeHandle, [this]()
	{
		ResetToIdle();
		WeaponState = bIsFiring ? EWeaponState::Fire : EWeaponState::Idle;
	}, WeaponList[idx]->GetData()->EquipDuration, false);
	
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeaponIdx = idx;
	CurrentWeapon = WeaponList[idx];
	CurrentWeapon->SetActorHiddenInGame(false);
}

void UWeaponComponent::ResetToIdle()
{
	WeaponState = EWeaponState::Idle;
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChangeHandle);
	AimEnd();
	FireTime = CurrentWeapon->GetData()->FireRate;
}
