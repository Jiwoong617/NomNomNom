// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/AutoLight.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "Core/NomPlayer.h"
#include "Engine/Light.h"

// Sets default values
AAutoLight::AAutoLight()
{
	PrimaryActorTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	RootComponent = TriggerBox;
}

// Called when the game starts or when spawned
void AAutoLight::BeginPlay()
{
	Super::BeginPlay();

	// Turn off all lights initially
	for (ALight* LightActor : LightsActor)
	{
		if (LightActor && LightActor->GetLightComponent())
		{
			LightActor->GetLightComponent()->SetIntensity(0.0f);
			LightActor->GetLightComponent()->SetVisibility(false);
		}
	}

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoLight::OnOverlapBegin);
	
}

void AAutoLight::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 트리거가 발동되었거나, 오버랩된 액터가 유효하지 않거나, 자기 자신이면 무시
	if (bHasBeenTriggered || !OtherActor || OtherActor == this)
	{
		return;
	}
	
	ANomPlayer* Player = Cast<ANomPlayer>(OtherActor);
	
	if (Player != nullptr)
	{
		
		bHasBeenTriggered = true;
		
		ActivateLight();
	}
}

void AAutoLight::ActivateLight()
{
	if (LightsActor.IsValidIndex(CurrentLightIndex))
	{
		ALight* LightActor = LightsActor[CurrentLightIndex];
		if (LightActor && LightActor->GetLightComponent())
		{
			LightActor->GetLightComponent()->SetVisibility(true);
			LightActor->GetLightComponent()->SetIntensity(5000.0f);
		}
		CurrentLightIndex++;

		if (LightsActor.IsValidIndex(CurrentLightIndex))
		{
			GetWorld()->GetTimerManager().SetTimer(LightTimer,this, &AAutoLight::ActivateLight,DelayBetweenLight,false);
		}
	}
}

// Called every frame
void AAutoLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
