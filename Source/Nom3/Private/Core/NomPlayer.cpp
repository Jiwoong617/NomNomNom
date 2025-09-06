// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/NomPlayer.h"

#include <rapidjson/document.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Nom3/Nom3.h"
#include "Weapon/WeaponComponent.h"

// Sets default values
ANomPlayer::ANomPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TPS Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -87));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

		GetMesh()->SetOwnerNoSee(true);
	}

	//FSP Mesh
	FpsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS Mesh"));
	FpsMeshComp->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempFpsMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (TempFpsMesh.Succeeded())
	{
		FpsMeshComp->SetSkeletalMesh(TempFpsMesh.Object);
		FpsMeshComp->SetRelativeLocation(FVector(0, 0, -87));
		FpsMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
		FpsMeshComp->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

		//내 카메라에만 보이게
		FpsMeshComp->SetOnlyOwnerSee(true);
	}
	
	//FPS Cam Settings
	FpsSpringArmComp = CreateDefaultSubobject<USpringArmComponent>("FPS Spring Arm");
	FpsSpringArmComp->SetupAttachment(RootComponent);
	FpsSpringArmComp->SetRelativeLocation(FVector(320.000000,0.000000,80.000000));
	FpsCameraComp = CreateDefaultSubobject<UCameraComponent>("FPS Cam");
	FpsCameraComp->SetupAttachment(FpsSpringArmComp);
	FpsCameraComp->bUsePawnControlRotation = true;
	FpsCameraComp->bEnableFirstPersonFieldOfView = true;
	FpsCameraComp->bEnableFirstPersonScale = true;
	FpsCameraComp->FirstPersonFieldOfView = 70.0f;
	FpsCameraComp->FirstPersonScale = 0.6f;

	//Basic Init
	JumpMaxCount = 2;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpForce;
	GetCharacterMovement()->GravityScale *= GravityMultiplier;
	GetCharacterMovement()->AirControl = 0.5f;

	//Add Component
	WeaponComp = CreateDefaultSubobject<UWeaponComponent>("WeaponComp");
}

// Called when the game starts or when spawned
void ANomPlayer::BeginPlay()
{
	Super::BeginPlay();
	auto pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto subsys = ULocalPlayer::GetSubsystem
		<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(IMC_NomPlayer, 0);
		}
	}

}

// Called every frame
void ANomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANomPlayer::MoveInput);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANomPlayer::LookInput);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ANomPlayer::JumpInput);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ANomPlayer::RunToggle);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ANomPlayer::CrouchToggle);

		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &ANomPlayer::InteractHold);

		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ANomPlayer::Fire);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ANomPlayer::Fire);
		
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &ANomPlayer::Aim);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &ANomPlayer::Aim);
		
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &ANomPlayer::Reload);
		
		EnhancedInputComponent->BindAction(IA_Melee, ETriggerEvent::Started, this, &ANomPlayer::Melee);
		EnhancedInputComponent->BindAction(IA_Throw, ETriggerEvent::Started, this, &ANomPlayer::Throw);
		EnhancedInputComponent->BindAction(IA_Skill, ETriggerEvent::Started, this, &ANomPlayer::Skill);
		EnhancedInputComponent->BindAction(IA_Ultimate, ETriggerEvent::Started, this, &ANomPlayer::UltimateSkill);

		EnhancedInputComponent->BindAction(IA_Weapon1, ETriggerEvent::Started, this, &ANomPlayer::ChangeWeapon1);
		EnhancedInputComponent->BindAction(IA_Weapon2, ETriggerEvent::Started, this, &ANomPlayer::ChangeWeapon2);
		EnhancedInputComponent->BindAction(IA_Weapon3, ETriggerEvent::Started, this, &ANomPlayer::ChangeWeapon3);
	}
}

void ANomPlayer::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MoveFunc(MovementVector.X, MovementVector.Y);
}

void ANomPlayer::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookFunc(LookAxisVector.X, LookAxisVector.Y);
}

void ANomPlayer::JumpInput()
{
	Jump();
}

void ANomPlayer::RunToggle()
{
	bIsRunning = !bIsRunning;
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? MaxSpeed * 2 : MaxSpeed;
}

void ANomPlayer::CrouchToggle()
{
	CrouchOrSlide();
	bIsCrouching = !bIsCrouching;
}

void ANomPlayer::LookFunc(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ANomPlayer::MoveFunc(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ANomPlayer::CrouchOrSlide()
{
	if (bIsRunning)
	{
		//슬라이드
		PRINTINFO();
	}
	else
	{
		if (bIsCrouching)
			UnCrouch();
		else
			Crouch();
	}
}

void ANomPlayer::InteractHold(const FInputActionValue& Value)
{
	float HoldDuration = Value.Get<float>();
	if (true)
	{
		InteractDuration = HoldDuration;
		// TODO : 에이밍 중이면
		if (HoldDuration >= 1.f)
		{
			PRINTINFO();
			InteractDuration = 0.f;
		}
	}
	else
	{
		InteractDuration = 0.f;
	}
}

void ANomPlayer::Fire(const FInputActionValue& Value)
{
	bool isFireing = Value.Get<bool>();
	if (isFireing)
	{
		WeaponComp->FireStart();
	}
	else
	{
		WeaponComp->FireEnd();
	}
}

void ANomPlayer::Aim(const FInputActionValue& Value)
{
	bool isAiming = Value.Get<bool>();
	if (isAiming)
	{
		WeaponComp->ReloadStart();
	}
	else
	{
		WeaponComp->ReloadEnd();
	}
}

void ANomPlayer::Reload()
{
	WeaponComp->ReloadStart();
}

void ANomPlayer::Melee()
{
	PRINTINFO();
}

void ANomPlayer::Throw()
{
	PRINTINFO();
}

void ANomPlayer::Skill()
{
	PRINTINFO();
}

void ANomPlayer::UltimateSkill()
{
	PRINTINFO();
}

void ANomPlayer::ChangeWeapon1()
{
	WeaponComp->ChangeWeapon(0);
}

void ANomPlayer::ChangeWeapon2()
{
	WeaponComp->ChangeWeapon(1);
}

void ANomPlayer::ChangeWeapon3()
{
	WeaponComp->ChangeWeapon(2);
}

USpringArmComponent* ANomPlayer::GetFpsCamArm()
{
	return FpsSpringArmComp;
}

UCameraComponent* ANomPlayer::GetFpsCam()
{
	return FpsCameraComp;
}

