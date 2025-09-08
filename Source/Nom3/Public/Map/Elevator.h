#pragma once

#include "CoreMinimal.h"
#include "Map/MovingObject.h"
#include "Interfaces/Interactable.h" 
#include "Elevator.generated.h"


UCLASS()
class NOM3_API AElevator : public AMovingObject, public IInteractable
{
	GENERATED_BODY()

public:
	// 플레이어 상호작용 시 호출될 함수입니다. 
	virtual void Interact_Implementation(AActor* Interactor) override;
};