// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/DamageActor.h"
#include "Components/WidgetComponent.h"
#include "Enemy/Damage/DamageActorPoolGameInstanceSubsystem.h"
#include "Enemy/Damage/DamageWidget.h"
#include "Nom3/Nom3.h"

ADamageActor::ADamageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//위젯 컴포넌트 부착
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetupAttachment(GetRootComponent());
}

void ADamageActor::BeginPlay()
{
	Super::BeginPlay();

	//위젯 컴포넌트의 위젯 캐스팅
	if (auto Temp = Cast<UDamageWidget>(WidgetComp->GetWidget()))
	{
		//프로퍼티에 할당
		DamageWidget = Temp;

		//풀에 자신을 반환하는 메서드를 람다식으로 바인딩
		DamageWidget->OnAnimationFinishedDelegate.AddLambda([this](){ Deactivate(); });
	}
}

void ADamageActor::Activate()
{
	//애니메이션 재생
	DamageWidget->PlayShowAndHideAnimation();
}

void ADamageActor::Deactivate()
{
	if (const auto GameInstance = GetGameInstance())
	{
		if (const auto PoolingSubsystem = GameInstance->GetSubsystem<UDamageActorPoolGameInstanceSubsystem>())
		{
			PoolingSubsystem->PushDamageActorToPool(this);
		}
	}
}