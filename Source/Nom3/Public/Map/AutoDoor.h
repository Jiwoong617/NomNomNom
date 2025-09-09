// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
<<<<<<< HEAD
#include "Map/MovingObject.h"
#include "Nom3/Public/Interfaces/Interactable.h"
=======
#include "MovingObject.h"
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
#include "AutoDoor.generated.h"

UCLASS()
class NOM3_API AAutoDoor : public AMovingObject
{
	GENERATED_BODY()

public:
<<<<<<< HEAD
	virtual void Interact_Implementation(AActor* Interactor) override;
=======
	AAutoDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Door")
	class UBoxComponent* TriggerBox;

	//문이 닫히기까지의 시간
	UPROPERTY(EditAnywhere, Category = "Door")
	float CloseDelay = 3.0f;
	
	//최대 속도까지 도달하는 가속도
	UPROPERTY(EditAnywhere, Category = "Door")
	float Acceleration = 2.0f;

private:
	//원래 설정된 최대 속도를 저장할 변수
	float MaxSpeed;

	//문이 열려 있어야 하는지 여부
	bool bShouldBeOpen = false;
	
	FTimerHandle CloseTimerHandle;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void CloseDoor();
>>>>>>> parent of 3fb5483 ([MAP] moving 관련 코드 디버깅)
};