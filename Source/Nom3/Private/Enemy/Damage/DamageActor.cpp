// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Damage/DamageActor.h"

#include "Components/WidgetComponent.h"
#include "Enemy/Damage/DamageWidget.h"

ADamageActor::ADamageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//위젯 컴포넌트 부착
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetupAttachment(GetRootComponent());

	//위젯 블루프린트 로드
	if (const ConstructorHelpers::FClassFinder<UDamageWidget> Finder(TEXT("/Game/Enemies/Damage/WBP_DamageWidget.WBP_DamageWidget_C"));
		Finder.Class)
	{
		WidgetComp->SetWidgetClass(Finder.Class);
	}
}

void ADamageActor::ShowAndHide()
{
	Cast<UDamageWidget>(WidgetComp->GetWidget())->ShowAndHide();

	
}
