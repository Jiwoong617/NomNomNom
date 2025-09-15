// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerFpsAnimation.generated.h"

enum class EActionState : uint8;
enum class EMovingState : uint8;

class ANomPlayer;
/**
 * 
 */
UCLASS()
class NOM3_API UPlayerFpsAnimation : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ANomPlayer* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMovingState MovingState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EActionState ActionState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Dir = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraRotX = 0.f;
};
