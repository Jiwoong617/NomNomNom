// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Core/DamageComponent.h"
#include "Core/NomPlayer.h"
#include "Interfaces/Damagable.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponData.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase() : WeaponData(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMeshComp;
	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> eff(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/FX/muzzle/N_MuzzleFalsh.N_MuzzleFalsh'"));
	if (eff.Succeeded())
		fireEffect = eff.Object;
		
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> impacteff(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/FX/NS_BulletEffect.NS_BulletEffect'"));
	if (impacteff.Succeeded())
		BulletImpactEffect = impacteff.Object;
		
	static ConstructorHelpers::FObjectFinder<UAnimMontage> reloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/gun/HandCannonReload.HandCannonReload'"));
	if (reloadMontage.Succeeded())
		ReloadMontage = reloadMontage.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> changeMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/Character/Character/gun/HandCannonEquip.HandCannonEquip'"));
	if (changeMontage.Succeeded())
		ChangeWeaponMontage = changeMontage.Object;
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

	if (IsHidden() == false)
	{
		ApplyingRecoil();
	}
}

void AWeaponBase::ApplyRecoil()
{
	const float RecoilPitch = FMath::RandRange(WeaponData->RecoilPitchMin, WeaponData->RecoilPitchMax);
	const float RecoilYaw = FMath::RandRange(WeaponData->RecoilYawMin, WeaponData->RecoilYawMax) *
		(FMath::RandBool() ? 1.f : -1.f);
	
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

void AWeaponBase::AimFire()
{
	if (CurrentAmmo > 0)
	{
		//애니메이션 재생
		if (GunShotMontage)
			WeaponOwner->PlayGunshotAnim(GunShotMontage);

		if (FireSound)
		{
			//사격 사운드 재생
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());	
		}

		//탄약 소모
		CurrentAmmo--;

		const FVector Pos = WeaponOwner->GetFpsCam()->GetComponentLocation();
		const FVector Dir = WeaponOwner->GetFpsCam()->GetForwardVector();
		
		FHitResult Hit;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(WeaponOwner);
		
		if (GetWorld()->LineTraceSingleByChannel(Hit,Pos,Pos + Dir * 10000, ECC_Visibility, Params))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Hit.GetComponent()->GetName());
			
			if (auto dmg = Cast<UDamageComponent>(Hit.GetComponent()))
			{
				PRINTLOG(TEXT("Damagable"));
				int32 damage = FMath::RandRange(WeaponData->Damage-50, WeaponData->Damage + 50);
				dmg->OnDamaged(FFireInfo(damage,
					WeaponMeshComp->GetSocketLocation(FireSocketName), ETeamInfo::Player, false));
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletImpactEffect,
				Hit.Location, Hit.Normal.Rotation());
			//DrawDebugSphere(GetWorld(), Hit.Location, 10, 1, FColor::Red, false, 3);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireEffect,
			WeaponMeshComp->GetSocketLocation(FireSocketName), WeaponMeshComp->GetSocketRotation(FireSocketName));
		
		DrawDebugLine(GetWorld(), Pos, Dir * 10000, FColor::Red, false, 1);
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%d/%d - %d"), CurrentAmmo, WeaponData->AmmoCount, MaxAmmo));

		//Recoil
		ApplyRecoil();
	}
}

void AWeaponBase::NoAimFire()
{
	if (CurrentAmmo > 0)
	{
		//애니메이션 재생
		if (GunShotMontage &&WeaponOwner)
			WeaponOwner->PlayGunshotAnim(GunShotMontage);

		if (FireSound)
		{
			//사격 사운드 재생
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());	
		}

		//탄약 소모
		CurrentAmmo--;

		//탄퍼짐
		const float SpreadHalf = WeaponData->BulletSpread * 0.5f;
		const float RandPitch = FMath::RandRange(-SpreadHalf, SpreadHalf);
		const float RandYaw = FMath::RandRange(-SpreadHalf, SpreadHalf);

		const FVector Pos = WeaponOwner->GetFpsCam()->GetComponentLocation();
		const FVector Dir = WeaponOwner->GetFpsCam()->GetForwardVector();
		FRotator SpreadRot = Dir.Rotation();
		SpreadRot.Pitch += RandPitch;
		SpreadRot.Yaw += RandYaw;
		const FVector FinalDir = SpreadRot.Vector();

		FHitResult Hit;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(WeaponOwner);

		if (GetWorld()->LineTraceSingleByChannel(Hit,Pos,Pos + Dir * 10000, ECC_Visibility, Params))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Hit.GetComponent()->GetName());
			
			if (auto dmg = Cast<UDamageComponent>(Hit.GetComponent()))
			{
				PRINTLOG(TEXT("Damagable"));
				int32 damage = FMath::RandRange(WeaponData->Damage-50, WeaponData->Damage + 50);
				dmg->OnDamaged(FFireInfo(damage,
					WeaponMeshComp->GetSocketLocation(FireSocketName), ETeamInfo::Player, false));
			}
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletImpactEffect,
				Hit.Location, Hit.Normal.Rotation());
			//DrawDebugSphere(GetWorld(), Hit.Location, 10, 1, FColor::Red, false, 3);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireEffect,
			WeaponMeshComp->GetSocketLocation(FireSocketName), WeaponMeshComp->GetSocketRotation(FireSocketName));
		
		DrawDebugLine(GetWorld(), Pos, Pos + FinalDir * 10000, FColor::Red, false, 1);
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%d/%d - %d"), CurrentAmmo, WeaponData->AmmoCount, MaxAmmo)
		);

		//Recoil
		ApplyRecoil();
	}
}

void AWeaponBase::FireEnd()
{
	if (ShellSound)
	{
		//탄피 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShellSound, GetActorLocation());
	}
}

void AWeaponBase::Reload()
{
	//부족한 탄약 만큼 탄창에 채운다
	const int32 NeededAmmo = FMath::Min(MaxAmmo, WeaponData->AmmoCount - CurrentAmmo);
	CurrentAmmo += NeededAmmo;
	MaxAmmo -= NeededAmmo;
}

void AWeaponBase::ReloadStart()
{
	//재장전 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
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
	return WeaponMeshComp->GetSocketTransform(SocketName, RTS_World);
}

bool AWeaponBase::CanFire()
{
	return CurrentAmmo != 0;
}

bool AWeaponBase::CanReload()
{
	return (CurrentAmmo != WeaponData->AmmoCount && MaxAmmo != 0);
}
