 #pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveButton.generated.h"

class UBoxComponent;
class ATwoButtonDoor;
class ANomPlayer;

UCLASS()
class NOM3_API AInteractiveButton : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveButton();

public:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerBox;

	// 에디터에서 연결할 문
	UPROPERTY(EditAnywhere, Category = "Button")
	ATwoButtonDoor* TargetDoor;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool		bFromSweep, const FHitResult& SweepResult);

};