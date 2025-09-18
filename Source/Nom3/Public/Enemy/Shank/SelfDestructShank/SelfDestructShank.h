// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Shank/Common/ShankBase.h"
#include "SelfDestructShank.generated.h"

UCLASS()
class NOM3_API ASelfDestructShank : public AShankBase
{
	GENERATED_BODY()

public:
	ASelfDestructShank();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnShotDown(const FVector ShotDir) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShankStateMachineBase> FinalAssaultStateMachine;
	
};
