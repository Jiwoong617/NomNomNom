// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CriticalDamagable.generated.h"

//전방 선언
struct FFireInfo;

// This class does not need to be modified.
UINTERFACE()
class UCriticalDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 크리티컬 데미지를 입을 수 있는 액터를 위한 인터페이스
 */
class NOM3_API ICriticalDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnCriticalDamaged(FFireInfo Info) = 0;
};