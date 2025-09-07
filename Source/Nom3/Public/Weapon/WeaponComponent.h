// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "WeaponComponent.generated.h"


class ANomPlayer;
class AWeaponBase;
enum class EWeaponType : uint8;
class UWeaponData;
struct FTimeline;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Fire,
	Reload,
	Changing
};

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

	//Shoot 관련
	UPROPERTY(EditAnywhere)
	EWeaponState WeaponState = EWeaponState::Idle;
	FTimerHandle ReloadHandle;
	FTimerHandle ChangeHandle;
	float FireTime = 0;
	bool bIsFiring = false;
	bool bIsAim = false;

	
	//Aim
	UPROPERTY(EditAnywhere)
	float CameraFOV = 90.f;
	UPROPERTY(EditAnywhere)
	FVector CamOffset;
	UPROPERTY(EditAnywhere)
	FVector AimCamLoc;
	UPROPERTY(EditAnywhere)
	FTimeline AimTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* AimCurve;
	
public:
	UFUNCTION()
	void Init();
	
	UFUNCTION()
	void FireStart();
	UFUNCTION()
	void FireEnd();
	UFUNCTION()
	void Fire();
	
	UFUNCTION()
	void ReloadStart();
	UFUNCTION()
	void ReloadEnd();
	
	UFUNCTION()
	void AimStart();
	UFUNCTION()
	void AimEnd();
	UFUNCTION()
	void OnAiming(float Value);
	
	UFUNCTION()
	void ChangeWeapon(int32 idx);
	UFUNCTION()
	void ResetToIdle();
};