#include "Core/ProjectilePoolWorldSubSystem.h"
#include "Enemy/Core/ProjectileBase.h"
#include "Engine/World.h"

void UProjectilePoolWorldSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UProjectilePoolWorldSubSystem::Deinitialize()
{
	//풀을 순회하면서 유효한 모든 발사체 액터를 파괴
	for (auto& Pair : ProjectilePools)
	{
		while (Pair.Value.IsEmpty() == false)
		{
			if (AProjectileBase* Pop = nullptr; Pair.Value.Dequeue(Pop))
			{
				Pop->Destroy();
			}
		}
	}

	//청소
	ProjectilePools.Empty();

	//종료
	Super::Deinitialize();
}

AProjectileBase* UProjectilePoolWorldSubSystem::PopProjectile(const TSubclassOf<AProjectileBase> Class, const FVector& Location, const FRotator& Rotation)
{
	if (Class == nullptr)
	{
		return nullptr;
	}

	//클래스를 이용해 적절한 큐로 접근
	TQueue<AProjectileBase*>& Pool = ProjectilePools.FindOrAdd(Class);

	//반환 공간
	AProjectileBase* Projectile = nullptr;

	//추출에 실패했다면
	if (Pool.Dequeue(Projectile) == false)
	{
		//새롭게 생성
		Projectile = SpawnRequestedProjectile(Class, Location, Rotation);
	}

	//활성화
	Projectile->Active(Location, Rotation);

	//반환
	return Projectile;
}

void UProjectilePoolWorldSubSystem::PushProjectile(AProjectileBase* Projectile)
{
	//유효하지 않다면
	if (Projectile == nullptr)
	{
		return;
	}

	//활성화
	Projectile->Inactivate();

	//클래스에 맞춰 새로운 큐를 생성하거나 검색
	TQueue<AProjectileBase*>& Pool = ProjectilePools.FindOrAdd(Projectile->GetClass());

	//큐에 삽입
	Pool.Enqueue(Projectile);
}

AProjectileBase* UProjectilePoolWorldSubSystem::SpawnRequestedProjectile(const TSubclassOf<AProjectileBase> Class, const FVector& Location, const FRotator& Rotation) const
{
	//스폰
	AProjectileBase* Spawned = GetWorld()->SpawnActor<AProjectileBase>(Class, Location, Rotation);

	//반환
	return Spawned;
}