#pragma once

#include "CoreMinimal.h"
#include "Map/MovingObject.h"
#include "Nom3/Public/Interfaces/Interactable.h"
#include "AutoDoor.generated.h"


UCLASS()
class NOM3_API AAutoDoor : public AMovingObject, public IInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
};