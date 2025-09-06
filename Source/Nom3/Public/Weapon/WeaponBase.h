// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
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

	UPROPERTY(EditAnywhere)
	FName FireSocketName = TEXT("FireSocket");

public:
	UPROPERTY(EditAnywhere)
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere)
	int32 MaxAmmo;
	
public:
	virtual void Fire();
	virtual void AimFire();
	virtual void Reload();

	void SetOwner(ANomPlayer* NewOwner);
	const UWeaponData* GetData() const;
	FTransform GetSocketTransform(FName& SocketName);
};
