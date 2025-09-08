// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NomPlayer.generated.h"

class UWeaponComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class USceneComponent;

// UENUM(BlueprintType)
// enum class EPlayerState : uint8
// {
// 	Idle = 0		UMETA(DisplayName = "Idle"),
// 	Walking	= 1		UMETA(DisplayName = "Walking"),
// 	Running = 2		UMETA(DisplayName = "Running"),
// 	Reloading = 3	UMETA(DisplayName = "Reloading"),
// 	Aiming = 4		UMETA(DisplayName = "Aiming"),
// 	Firing = 5		UMETA(DisplayName = "Firing"),
// 	AimFire = 6		UMETA(DisplayName = "AimFire"),
// 	Crouch = 7		UMETA(DisplayName = "Crouch"),
// };

UCLASS()
class NOM3_API ANomPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANomPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	//Basic Status
	float MaxSpeed = 800.f;
	float JumpForce = 600.f;
	float GravityMultiplier = 0.75f;

	//Movement params
	bool bIsFiring = false;
	bool bIsRunning = false;
	bool bIsCrouching = false;
	bool bIsAiming = false;
	float InteractDuration = 0.f;
	
	//Fps Mesh
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FpsMeshComp;
	
	//Camera and Weapon
	UPROPERTY(EditAnywhere)
	USpringArmComponent* FpsSpringArmComp;
	UPROPERTY(EditAnywhere)
	UCameraComponent* FpsCameraComp;
	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponListSceneComp;

	//Components
	UPROPERTY(EditAnywhere)
	UWeaponComponent* WeaponComp;
	
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
	UInputAction* IA_Weapon1;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Weapon2;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Weapon3;

	//Reload Timer
	FTimerHandle ReloadHandle;
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
	UFUNCTION()
	void CrouchOrSlide();

	//interaction
	UFUNCTION()
	void InteractHold(const FInputActionValue& Value);

	//Battle
	UFUNCTION()
	void Fire(const FInputActionValue& Value);
	UFUNCTION()
	void Aim(const FInputActionValue& Value);
	UFUNCTION()
	void ReloadStart();
	UFUNCTION()
	void ReloadEnd();
	UFUNCTION()
	virtual void Melee();
	UFUNCTION()
	virtual void Throw();
	UFUNCTION()
	virtual void Skill();
	UFUNCTION()
	virtual void UltimateSkill();

	UFUNCTION()
	void ChangeWeapon1();
	UFUNCTION()
	void ChangeWeapon2();
	UFUNCTION()
	void ChangeWeapon3();

	//OnCanceled - Moving
	UFUNCTION() void OnRunCanceled();
	UFUNCTION() void OnCrouchCanceled();
	//OnCancled - Weapon
	UFUNCTION() void OnFireCanceled();
	UFUNCTION() void OnReloadCanceled();
	UFUNCTION() void OnAimCanceled();
	UFUNCTION() void OnChangeWeaponCanceled();

public:
	USpringArmComponent* GetFpsCamArm();
	UCameraComponent* GetFpsCam();
};
