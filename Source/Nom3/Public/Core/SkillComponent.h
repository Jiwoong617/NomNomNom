// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


class ANomPlayer;
class USkillBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCoolDownDelegate, float, cool, float, coolTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOM3_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	ANomPlayer* Owner;
	
    UPROPERTY(EditAnywhere, Instanced) USkillBase* DodgeSkill;
    UPROPERTY(EditAnywhere, Instanced) USkillBase* UltimateSkill;

    float DodgeCooldown = 0.f;
    float UltimateCooldown = 0.f;
	
public:
    FOnSkillCoolDownDelegate DodgeSkillCoolDownDelegate;
    FOnSkillCoolDownDelegate UltimateSkillCoolDownDelegate;

    // 스킬 사용 시도: 시작되면 true, 쿨중이면 false
    bool UseDodgeSkill();
    bool UseUltimateSkill();
};
