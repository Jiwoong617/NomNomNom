// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Components/FireProjectileComponentBase.h"
#include "Nom3/Nom3.h"

UFireProjectileComponentBase::UFireProjectileComponentBase() :
	ShootRandConeAngle(1),
	FireCount(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UFireProjectileComponentBase::ActiveAutoFire()
{
	//타이머 설정
	GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, [this]()
	{
		//자동 사격 메서드 호출
		FireBulletMultipleForInnerReq(0.25);
	}, 4, true);
}

void UFireProjectileComponentBase::InactiveAutoFire()
{
	//사격 타이머 비활성화
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
}

void UFireProjectileComponentBase::FireBulletMultipleForInnerReq(const float Delay)
{
	//사격 횟수 결정
	const int Num = FMath::RandRange(1, 5);

	//요청 수행
	FireBulletMultiple(Num, Delay);
}

void UFireProjectileComponentBase::FireBulletMultipleForOuterReq(const int Num, const float Delay)
{
	//이미 자동 사격 상태이므로 외부 요청을 무시한다
	if (AutoFireTimerHandle.IsValid())
	{
		return;
	}

	//요청 수행
	FireBulletMultiple(Num, Delay);
}

void UFireProjectileComponentBase::FireBulletMultiple(const int Num, const float Delay)
{
	//사격 카운트
	FireCount = 0;

	//사격 개시
	GetWorld()->GetTimerManager().SetTimer(FireIntervalTimerHandle, [this, Num]()
	{
		//사격 종료
		if (FireCount >= Num)
		{
			GetWorld()->GetTimerManager().ClearTimer(FireIntervalTimerHandle);
			return;
		}

		//자동 사격 호출
		FireBulletOnce();

		//사격 횟수 증가
		FireCount++;
	}, Delay, true);
}

void UFireProjectileComponentBase::FireBulletOnce() const
{
	PRINTLOG(TEXT("FatalError! You Should Fully Override This Method!"));
}