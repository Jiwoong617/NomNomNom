// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"


#include "Core/NomPlayer.h"
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
}

void UWeaponComponent::FireStart()
{
	CurrentWeapon->FireStart();
}

void UWeaponComponent::FireEnd()
{
	CurrentWeapon->FireEnd();
}

void UWeaponComponent::ReloadStart()
{
	CurrentWeapon->ReloadStart();
}

void UWeaponComponent::ReloadEnd()
{
	CurrentWeapon->ReloadEnd();
}

void UWeaponComponent::AimStart()
{
	CurrentWeapon->AimStart();
}

void UWeaponComponent::AimEnd()
{
	CurrentWeapon->AimEnd();
}

void UWeaponComponent::ChangeWeapon(int32 idx)
{
	if (idx == CurrentWeaponIdx)
	{
		return;
	}
	
	AimEnd();
	
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeaponIdx = idx;
	CurrentWeapon = WeaponList[idx];
	CurrentWeapon->SetActorHiddenInGame(false);
}

void UWeaponComponent::OnFireCanceled()
{
}

void UWeaponComponent::OnReloadCanceled()
{
}

void UWeaponComponent::OnAimCanceled()
{
}

void UWeaponComponent::OnChangeWeaponCanceled()
{
}

AWeaponBase* UWeaponComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}
