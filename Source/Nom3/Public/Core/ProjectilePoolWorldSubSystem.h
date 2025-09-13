#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Containers/Queue.h"
#include "ProjectilePoolWorldSubSystem.generated.h"

class AProjectileBase;

UCLASS()
class NOM3_API UProjectilePoolWorldSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION()
	AProjectileBase* PopProjectile(TSubclassOf<AProjectileBase> Class, const FVector& Location, const FRotator& Rotation);

	UFUNCTION()
	void PushProjectile(AProjectileBase* Projectile);

private:
	//클래스를 토대로 발사체를 저장하는 풀링
	TMap<UClass*, TQueue<AProjectileBase*>> ProjectilePools;

	//요청한 발사체를 스폰한다
	UFUNCTION()
	AProjectileBase* SpawnRequestedProjectile(TSubclassOf<AProjectileBase> Class, const FVector& Location, const FRotator& Rotation) const;
};
