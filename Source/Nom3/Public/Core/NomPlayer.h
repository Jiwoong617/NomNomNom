// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CriticalDamagable.h"
#include "Interfaces/Damagable.h"
#include "NomPlayer.generated.h"

class UPlayerTpsAnimation;
class UPlayerFpsAnimation;
class UPlayerUI;
class UPlayerDamageComponent;
class UWeaponComponent;
class USkillComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class USceneComponent;

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Idle = 0		UMETA(DisplayName = "Idle"),
	Reloading = 1	UMETA(DisplayName = "Reloading"),
	ChangeWeapon = 2	UMETA(DisplayName = "ChangeWeapon"),
	Firing = 3		UMETA(DisplayName = "WeaponSwap"),
	LeftHand = 4	UMETA(DisplayName = "LeftHand"),
	Skill = 5		UMETA(DisplayName = "Skill"),
};

UENUM(BlueprintType)
enum class EMovingState : uint8
{
	Idle = 0		UMETA(DisplayName = "Idle"),
	Running = 1		UMETA(DisplayName = "Running"),
	Crouch = 2		UMETA(DisplayName = "Crouch"),
};

UCLASS()
class NOM3_API ANomPlayer : public ACharacter, public IDamagable, public ICriticalDamagable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANomPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	EActionState ActionState = EActionState::Idle;
	EMovingState MovingState = EMovingState::Idle;
	bool bIsAiming = false;
	bool bIsHoldFire = false;
	bool bIsHoldAim = false;
	bool bIsDead = false;
	
	//Basic Status
	float MaxSpeed = 1000.f;
	float JumpForce = 1200.f;
	float GravityMultiplier = 1.2f;

	//Movement params
	float InteractDuration = 0.f;
	
	//Fps Mesh
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TpsMeshComp;
	
	//Camera and Weapon
	UPROPERTY(EditAnywhere)
	USpringArmComponent* FpsSpringArmComp;
	UPROPERTY(EditAnywhere)
	UCameraComponent* FpsCameraComp;
	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponListSceneComp;

	UPROPERTY(EditAnywhere)
	FVector TpsCamOffset = FVector(0.f, 0.f, 50.f);
	UPROPERTY(EditAnywhere)
	float TpsSpringArmLength = 300.f;
	UPROPERTY(EditAnywhere)
	USpringArmComponent* TpsSpringArmComp;
	UPROPERTY(EditAnywhere)
	UCameraComponent* TpsCameraComp;
	
    //Components
    UPROPERTY(EditAnywhere)
    UWeaponComponent* WeaponComp;
    UPROPERTY(EditAnywhere)
    USkillComponent* SkillComp;
	
	//Inputs
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_NomPlayer;

	//Movement
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Run;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Crouch;
	
	// Interaction
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Interact;

	// Combat
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Aim;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Reload;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Melee;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Throw;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Skill;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Ultimate;
	
	//Weapon Swap
	UPROPERTY(EditAnywhere)
	UInputAction* IA_ChangeWeapon;

	FTimerHandle PutWeaponHandle;
	FTimerHandle ChangeWeaponHandle;

	//Damage, Hp
	int32 MaxHp = 50;
	int32 Hp = 50;
	float FistDamage = 100.f;
	UPROPERTY(EditAnywhere) UAnimMontage* PunchMontage;
	UPROPERTY(EditAnywhere) UPlayerDamageComponent* HeadBox;
	UPROPERTY(EditAnywhere) UPlayerDamageComponent* BodyBox;

	//UI
	TSubclassOf<UPlayerUI> PlayerUIClass;
	UPlayerUI* PlayerUI;

	//Animation
	UPlayerFpsAnimation* FpsAnimation;
	UPlayerTpsAnimation* TpsAnimation;
	
public:

protected:
	//Movement InputEvent
	UFUNCTION()
	void MoveInput(const FInputActionValue& Value);
	UFUNCTION()
	void LookInput(const FInputActionValue& Value);
	UFUNCTION()
	void JumpInput();
	UFUNCTION()
	void RunToggle();
	UFUNCTION()
	void CrouchToggle();
	
	//MoveMent Func
	UFUNCTION()
	virtual void LookFunc(float Yaw, float Pitch);
	UFUNCTION()
	virtual void MoveFunc(float Right, float Forward);

	//interaction
	UFUNCTION()
	void InteractHold(const FInputActionInstance& Value);

	//Battle
	UFUNCTION()
	void Fire(const FInputActionValue& Value);
	UFUNCTION()
	void Aim(const FInputActionValue& Value);
	UFUNCTION()
	void ReloadStart();
	UFUNCTION()
	void ReloadEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void Melee(); //lefthand
	UFUNCTION()
	virtual void Throw(); //lefthand
	UFUNCTION()
	virtual void LeftHandEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void Skill();
	UFUNCTION()
	virtual void UltimateSkill();
	UFUNCTION()
	virtual void SkillEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void ChangeWeapon(const FInputActionValue& Value);
	UFUNCTION()
	void OnWeaponChanged(float Idx);

	//OnCancled - Weapon
	UFUNCTION() void OnFireCanceled();
	UFUNCTION() void OnReloadCanceled();
	UFUNCTION() void OnAimCanceled();

	//Camera
	UFUNCTION() void ChangeToFps();
	UFUNCTION() void ChangeToTps();
	UFUNCTION() void SightCheck();

	//Die and Respawn
	FName PrevMeshCollisionProfileName;
	UFUNCTION() void MakeTpsRagdoll();
	UFUNCTION() void ReSpawn();

public:
	UFUNCTION() virtual void OnDamaged(FFireInfo Info) override;
	UFUNCTION() virtual void OnCriticalDamaged(FFireInfo Info) override;
	USpringArmComponent* GetFpsCamArm();
	UCameraComponent* GetFpsCam();
	const EActionState& GetActionState() const;
	const EMovingState& GetMovingState() const;

	//Anim
	UFUNCTION()
	void PlayGunshotAnim(UAnimMontage* Montage);
	UFUNCTION()
	void PlayFPSAnim(UAnimMontage* Montage, EActionState action = EActionState::LeftHand, int32 Idx = 0);
	UFUNCTION()
	void PlayTPSAnim(UAnimMontage* Montage);
};
