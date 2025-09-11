// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/CriticalLeftDamageActor.h"
#include "Components/WidgetComponent.h"

ACriticalLeftDamageActor::ACriticalLeftDamageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//위젯 블루프린트 클래스 로드
	if (const ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Game/Enemies/Damage/WBP_CriticalLeftDamageWidget.WBP_CriticalLeftDamageWidget_C"));
		Finder.Class)
	{
		WidgetComp->SetWidgetClass(Finder.Class);
	}
}