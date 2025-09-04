// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NomPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;


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
	bool bIsRunning = false;
	bool bIsCrouching = false;
	bool bIsAiming = false;
	float InteractDuration = 0.f;
	
	//Fps Mesh
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FpsMeshComp;
	
	//Camera
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

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
	void Fire();
	UFUNCTION()
	void Aim();
	UFUNCTION()
	void Melee();
	UFUNCTION()
	void Throw();
	UFUNCTION()
	void Skill();
	UFUNCTION()
	void UltimateSkill();

	UFUNCTION()
	void ChangeWeapon1();
	UFUNCTION()
	void ChangeWeapon2();
	UFUNCTION()
	void ChangeWeapon3();
};
