// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class ANomPlayer;
class AWeaponBase;
enum class EWeaponType : uint8;
class UWeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBulletChangeDelegate, int32, currentammo, int32, maxammo);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	ANomPlayer* Owner;

	//Weapon
	UPROPERTY(EditAnywhere)
	TArray<AWeaponBase*> WeaponList;
	UPROPERTY(EditAnywhere)
	int32 CurrentWeaponIdx = 0;
	UPROPERTY(EditAnywhere)
	AWeaponBase* CurrentWeapon;

	//Socket Name
	UPROPERTY(EditAnywhere)
	FName LeftHandSocket = TEXT("LeftHandSocket");
	UPROPERTY(EditAnywhere)
	FName RightHandSocket = TEXT("RightHandSocket");
	UPROPERTY(EditAnywhere)
	FName AimSocket = TEXT("AimSocket");
	

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
	FOnBulletChangeDelegate OnBulletChangeDelegate;

private:
	UFUNCTION() void Init();

	UFUNCTION() void Fire();
	
	UFUNCTION() void OnAiming();
	
public:
	
	UFUNCTION() void FireStart();
	UFUNCTION() void FireEnd();
	
	UFUNCTION() void Reload();
	
	UFUNCTION() void AimStart();
	UFUNCTION() void AimEnd();
	
	UFUNCTION() void ChangeWeapon(int32 idx);
	UFUNCTION() void OnWeaponChanged(int32 idx);
	
	UFUNCTION()
	AWeaponBase* GetCurrentWeapon();
	UFUNCTION()
	int32 GetCurrentWeaponIdx();
};