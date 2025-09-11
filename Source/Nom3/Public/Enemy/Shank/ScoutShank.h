// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShankBase.h"
#include "ScoutShank.generated.h"

UCLASS()
class NOM3_API AScoutShank : public AShankBase
{
	GENERATED_BODY()

public:
	AScoutShank();

	//플레이어를 향해 총탄 발사
	UFUNCTION(BlueprintCallable)
	void FireBullet(FVector SightLocation) const;

	//플레이어 시선 노출 인터페이스 구현
	virtual void OnAimByPlayerSight() override;

protected:
	virtual void BeginPlay() override;

	//정찰 생크가 발사하는 총탄 클래스
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AScoutShankBullet> ScoutShankBulletClass;

public:
	virtual void Tick(float DeltaTime) override;
};