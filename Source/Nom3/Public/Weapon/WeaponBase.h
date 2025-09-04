// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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

private:
	UPROPERTY(EditDefaultsOnly)
	UWeaponData* WeaponData;

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;
	//TODO : 상속 받고 메쉬 적용해야됨
	
	UPROPERTY(EditAnywhere)
	FName LeftHandSocket = TEXT("LeftHandSocket");
	UPROPERTY(EditAnywhere)
	FName RightHandSocket = TEXT("RightHandSocket");
	UPROPERTY(EditAnywhere)
	FName AimSocket = TEXT("AimSocket");
	
public:
	virtual void InitializeWeapon(UWeaponData* InData);
	
	virtual void Fire();

	virtual void Reload();

	virtual void Aim();
};
