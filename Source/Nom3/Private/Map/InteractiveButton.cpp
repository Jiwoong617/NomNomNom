// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/InteractiveButton.h"
 #include "Components/BoxComponent.h"
 #include "Map/TwoButtonDoor.h"
 #include "Core/NomPlayer.h"

 AInteractiveButton::AInteractiveButton()
 {
 	PrimaryActorTick.bCanEverTick = false;

 	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
 	RootComponent = TriggerBox;

 	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveButton::OnOverlapBegin);
 	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractiveButton::OnOverlapEnd);
 }

void AInteractiveButton::BeginPlay()
 {
 	Super::BeginPlay();
 }

void AInteractiveButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
 	ANomPlayer* PlayerCharacter = Cast<ANomPlayer>(OtherActor);
 	if (PlayerCharacter && TargetDoor)
 	{
 		TargetDoor->ActivateButton(this);
 	}
 }

void AInteractiveButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
 {
 	ANomPlayer* PlayerCharacter = Cast<ANomPlayer>(OtherActor);
 	if (PlayerCharacter && TargetDoor)
 	{
 		TargetDoor->DeactivateButton(this);
 	}
 }