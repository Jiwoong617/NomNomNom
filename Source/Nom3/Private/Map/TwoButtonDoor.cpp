// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/TwoButtonDoor.h"
#include "Map/InteractiveButton.h"
#include "Nom3/Nom3.h"

ATwoButtonDoor::ATwoButtonDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    bIsButton1Active = false;
    bIsButton2Active = false;
}

void ATwoButtonDoor::BeginPlay()
{
    Super::BeginPlay();
}

// 버튼이 눌렸다면
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
    // 비활성화 따윈 없다
}


//
void ATwoButtonDoor::CheckDoorState()
{
    if (bIsButton1Active && bIsButton2Active)
    {
       PRINTLOG(TEXT("두 버튼 활성화: 문이 사라집니다."));
        
        // 비활성화 처리
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
}