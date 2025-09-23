// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Components/FireProjectileComponentBase.h"
#include "Nom3/Nom3.h"

UFireProjectileComponentBase::UFireProjectileComponentBase() :
	AutoFireRate(4),
	AutoFireMin(1),
	AutoFireMax(1),
	FireCount(0),
	FireLimit(0),
	FireInterval(0.25f),
	ShootRandConeAngle(1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UFireProjectileComponentBase::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//발사 카운트가 작은 동안
	if (FireCount < FireLimit)
	{
		//발사 간격을 넘겼으므로
		if (ElapsedTimeAfterLastFire > FireInterval)
		{
			//1회 사격 호출
			FireBulletOnce();
		
			//발사 카운트 증가
			FireCount++;

			//ElapsedTime 갱신
			ElapsedTimeAfterLastFire -= AutoFireRate;
		}

		//어쨌든 시간은 지나간다
		ElapsedTimeAfterLastFire += DeltaTime;
	}
}

void UFireProjectileComponentBase::ActiveAutoFire()
{
	//타이머 델리게이트 바인드
	AutoFireTimerDelegate.BindLambda([this]()
	{
		//자동 사격 메서드 호출
		FireBulletMultiple();
	});

	//자동 사격 호출
	GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, AutoFireRate, true);
}

void UFireProjectileComponentBase::InactiveAutoFire()
{
	//사격 타이머 비활성화
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
}

void UFireProjectileComponentBase::FireBulletMultiple()
{
	//사격 카운트
	FireCount = 0;

	//사격 횟수 결정
	FireLimit = FMath::RandRange(AutoFireMin, AutoFireMax);
}

void UFireProjectileComponentBase::FireBulletOnce() const
{
	PRINTLOG(TEXT("FatalError! You Should Fully Override This Method!"));
}