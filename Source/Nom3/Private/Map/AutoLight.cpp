// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/AutoLight.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "Core/NomPlayer.h"
#include "Engine/Light.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAutoLight::AAutoLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	RootComponent = TriggerBox;
}

// Called when the game starts or when spawned
void AAutoLight::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoLight::OnOverlapBegin);
	
}

void AAutoLight::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 트리거가 발동되었거나, 오버랩된 액터가 유효하지 않거나, 자기 자신이면 무시합니다.
	if (bHasBeenTriggered || !OtherActor || OtherActor == this)
	{
		return;
	}

	// 오버랩된 액터(OtherActor)가 ANomPlayer 클래스인지 확인합니다.
	ANomPlayer* Player = Cast<ANomPlayer>(OtherActor);

	// Cast에 성공하면 Player 포인터는 유효한 값을 가집니다. (성공했다는 것은 플레이어가 들어왔다는 의미)
	if (Player != nullptr)
	{
		// 한 번만 실행되도록 플래그를 설정합니다.
		bHasBeenTriggered = true;

		// 첫 번째 라이트를 즉시 켜거나, 타이머를 시작합니다.
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

