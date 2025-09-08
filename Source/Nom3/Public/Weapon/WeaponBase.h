// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ANomPlayer;
class UWeaponData;

UCLASS()
class NOM3_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere)
	const UWeaponData* WeaponData;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(VisibleAnywhere)
	ANomPlayer* WeaponOwner;

	//SocketName
	UPROPERTY(EditAnywhere)
	FName FireSocketName = TEXT("FireSocket");
	UPROPERTY(EditAnywhere)
	FName AimSocket = TEXT("AimSocket");
	
	//Recoil
	UPROPERTY(EditAnywhere)
	FRotator CurrentRecoil; //현재 반동
	UPROPERTY(EditAnywhere)
	FRotator TargetRecoil; //목표 반동
	UPROPERTY(EditAnywhere)
	FRotator LastAppliedRecoil; // 직전 프레임에 적용한 반동

	UPROPERTY(EditAnywhere)
	float FireTime;
	UPROPERTY(EditAnywhere)
	bool bIsAiming = false;
	UPROPERTY(EditAnywhere)
	bool bIsFiring = false;
	UPROPERTY(EditAnywhere)
	bool bIsReloading = false;

	//ADS
	UPROPERTY(EditAnywhere)
	float CameraFOV = 90.f;
	UPROPERTY(EditAnywhere)
	FVector CamOffset;
	UPROPERTY(EditAnywhere)
	FVector AimCamLoc;
	UPROPERTY(EditAnywhere)
	float AimTime;

public:
	UPROPERTY(EditAnywhere)
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere)
	int32 MaxAmmo;

protected:
	UFUNCTION() void ApplyRecoil();
	UFUNCTION() void ApplyingRecoil();

	UFUNCTION() virtual void AimFire();
	UFUNCTION() virtual void NoAimFire();
	
	void OnAiming();
	
public:
	UFUNCTION() void FireStart();
	UFUNCTION() void FireEnd();
	UFUNCTION() void Fire();
	
	UFUNCTION() void ReloadStart();
	UFUNCTION() void ReloadEnd();
	UFUNCTION() void Reload();
	
	UFUNCTION() void AimStart();
	UFUNCTION() void AimEnd();

	//OnCancled - Weapon
	UFUNCTION() void OnFireCanceled();
	UFUNCTION() void OnReloadCanceled();
	UFUNCTION() void OnAimCanceled();
	
	void SetOwner(ANomPlayer* NewOwner);
	const UWeaponData* GetData() const;
	FTransform GetSocketTransform(FName& SocketName);
};
