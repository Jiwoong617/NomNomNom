// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnAimByPlayerSight.generated.h"

UINTERFACE(Blueprintable)
class UOnAimByPlayerSight : public UInterface
{
	GENERATED_BODY()
};

/**
 * 플레이어 시선에 닿았을 때 동작을 구현하도록 강제하는 인터페이스. 회피, 반격, 상호작용 등의 특수한 동작을 구현한다.
 */
class NOM3_API IOnAimByPlayerSight
{
	GENERATED_BODY()

public:
	virtual void OnAimByPlayerSight() = 0;
};
