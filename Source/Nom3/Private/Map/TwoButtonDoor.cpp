// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/TwoButtonDoor.h"
#include "Map/InteractiveButton.h"
#include "Nom3/Nom3.h"

ATwoButtonDoor::ATwoButtonDoor()
{
    // 더 이상 Tick 함수를 사용하지 않으므로 비활성화합니다.
    PrimaryActorTick.bCanEverTick = false;

    bIsButton1Active = false;
    bIsButton2Active = false;
}

void ATwoButtonDoor::BeginPlay()
{
    Super::BeginPlay();
}

void ATwoButtonDoor::ActivateButton(AInteractiveButton* Button)
{
    if (Button == Button1)
    {
        bIsButton1Active = true;
        PRINTLOG(TEXT("Button 1 활성화"));
    }
    else if (Button == Button2)
    {
        bIsButton2Active = true;
        PRINTLOG(TEXT("Button 2 활성화"));
    }
    CheckDoorState();
}

void ATwoButtonDoor::DeactivateButton(AInteractiveButton* Button)
{
    // 현재 로직에서는 버튼이 비활성화되지 않습니다.
}

void ATwoButtonDoor::CheckDoorState()
{
    if (bIsButton1Active && bIsButton2Active)
    {
       PRINTLOG(TEXT("두 버튼 활성화: 문이 사라집니다."));
        
        // 액터를 숨기고 충돌을 비활성화합니다.
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
}