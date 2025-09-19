// Fill out your copyright notice in the Description page of Project Settings.


#include "Nom3/Public/Core/DestinyPlayerController.h"
#include "Nom3/Public/Core/ClearUI.h"


void ADestinyPlayerController::ShowClearUI()
{
	if (ClearUI)
	{
		UUserWidget* ClearUIwidget = CreateWidget<UUserWidget>(this,ClearUI);
		if (ClearUIwidget)
		{
			ClearUIwidget->AddToViewport();
		}
	}
}
