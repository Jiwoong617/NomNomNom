// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

UENUM(BlueprintType)
enum class ETeamInfo : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Player = 1		UMETA(DisplayName = "Player"),
	Enemy = 2		UMETA(DisplayName = "Enemy"),
};

USTRUCT(BlueprintType)
struct FFireInfo
{
	GENERATED_BODY()
	
	FFireInfo() :
		Damage(0.f),
		FireLocation(FVector::ZeroVector),
		TeamInfo(ETeamInfo::None),
		bIsIgnoreTeam(false),
		ProjectileSpeed(-1) {}

	FFireInfo(const float Damage, const FVector& Location, const ETeamInfo TeamInfo, const bool bIsIgnore) :
		Damage(Damage),
		FireLocation(Location),
		TeamInfo(TeamInfo),
		bIsIgnoreTeam(bIsIgnore),
		ProjectileSpeed(-1){}

	FFireInfo(const float Damage, const FVector& Location, const ETeamInfo TeamInfo, const bool bIsIgnore, const float Speed) :
		Damage(Damage),
		FireLocation(Location),
		TeamInfo(TeamInfo),
		bIsIgnoreTeam(bIsIgnore),
		ProjectileSpeed(Speed) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	FVector FireLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	ETeamInfo TeamInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	bool bIsIgnoreTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireInfo")
	TObjectPtr<UCurveFloat> DamageByDistanceCurve;
};

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOM3_API IDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//총탄 구조체 : 데미지, 발사 지점, 사격자의 팀 정보, 동일팀 무시여부
	virtual void OnDamaged(FFireInfo Info) = 0;
};