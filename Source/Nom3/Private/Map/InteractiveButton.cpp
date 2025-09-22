// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/InteractiveButton.h"
 #include "Components/BoxComponent.h"
 #include "Map/TwoButtonDoor.h"
 #include "Core/NomPlayer.h"
#include "Nom3/Nom3.h"


AInteractiveButton::AInteractiveButton()
 {
 	PrimaryActorTick.bCanEverTick = false;

 	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
 	RootComponent = TriggerBox;

 	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveButton::OnOverlapBegin);
 }

void AInteractiveButton::BeginPlay()
 {
    Super::BeginPlay();
    if (TargetDoor == nullptr)
    {
       PRINTLOG(TEXT("InteractiveButton: TargetDoor is not set for %s!"), *GetName());
    }
 }


// player 캐릭터가 닿았다면
void AInteractiveButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
 	ANomPlayer* PlayerCharacter = Cast<ANomPlayer>(OtherActor);
 	if (PlayerCharacter && TargetDoor)
 	{
 		TargetDoor->ActivateButton(this);
 		PRINTLOG(TEXT("활성화"))
 	}
 }

