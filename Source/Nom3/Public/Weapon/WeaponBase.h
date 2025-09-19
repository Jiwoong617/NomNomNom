// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ANomPlayer;
class UWeaponData;
class UNiagaraSystem;

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
	
	UPROPERTY(EditAnywhere)
	FName FireSocketName = TEXT("FireSocket");

	//Recoil
	UPROPERTY(EditAnywhere)
	FRotator CurrentRecoil; //현재 반동
	UPROPERTY(EditAnywhere)
	FRotator TargetRecoil; //목표 반동
	UPROPERTY(EditAnywhere)
	FRotator LastAppliedRecoil; // 직전 프레임에 적용한 반동
	
	//Test
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* fireEffect;
	
public:
	UPROPERTY(EditAnywhere)
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere)
	int32 MaxAmmo;
	
	//Anim
	UPROPERTY(EditAnywhere)
	UAnimMontage* GunShotMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ChangeWeaponMontage;

protected:
	UFUNCTION() void ApplyRecoil();
	UFUNCTION() void ApplyingRecoil();

	//발사 사운드
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> FireSound;

	//재장전 사운드
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> ReloadSound;

	//탄피 사운드
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> ShellSound;
	 
public:
	UFUNCTION() virtual void AimFire();
	UFUNCTION() virtual void NoAimFire();
	
	UFUNCTION() virtual void Reload();

	void SetOwner(ANomPlayer* NewOwner);
	const UWeaponData* GetData() const;
	FTransform GetSocketTransform(FName& SocketName);
	bool CanFire();
	bool CanReload();
};
