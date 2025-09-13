// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ProjectileBase.generated.h"

//전방 선언
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class NOM3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	//발사 구조체
	UPROPERTY(VisibleAnywhere)
	FFireInfo ProjectileFireInfo;

	//활성화
	UFUNCTION()
	void Active(const FVector& Location, const FRotator& Rotation);

	//비활성화
	UFUNCTION()
	void Inactivate();

protected:

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	//발사체 무브 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;
};