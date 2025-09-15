// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/NomPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "KismetTraceUtils.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Damagable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Nom3/Nom3.h"
#include "Weapon/Grenade.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponComponent.h"
#include "Weapon/WeaponData.h"
#include "Core/PlayerDamageComponent.h"
#include "Core/PlayerFpsAnimation.h"
#include "Core/PlayerUI.h"
#include "Enemy/Core/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

ANomPlayer::ANomPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//FPS Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Character/Character/NomPlayer.NomPlayer'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -87));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

		//내 카메라에만 보이게
		GetMesh()->SetOnlyOwnerSee(true);
		GetMesh()->SetupAttachment(RootComponent);
	}
	ConstructorHelpers::FClassFinder<UPlayerFpsAnimation> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/Player/ABP_Fps.ABP_Fps_C'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
		FpsAnimation = Cast<UPlayerFpsAnimation>(GetMesh()->GetAnimInstance());
	}


	//TPS Mesh
	TpsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPS Mesh"));
	TpsMeshComp->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempTpsMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (TempTpsMesh.Succeeded())
	{
		TpsMeshComp->SetSkeletalMesh(TempTpsMesh.Object);
		TpsMeshComp->SetRelativeLocation(FVector(0, 0, -87));
		TpsMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
	
		TpsMeshComp->SetOwnerNoSee(true);
	}

	
	//FPS Cam Settings
	FpsSpringArmComp = CreateDefaultSubobject<USpringArmComponent>("FPS Spring Arm");
	FpsSpringArmComp->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	FpsSpringArmComp->TargetArmLength = 0;
	//FpsSpringArmComp->SetRelativeLocation(FVector(60,0,90));
	FpsCameraComp = CreateDefaultSubobject<UCameraComponent>("FPS Cam");
	FpsCameraComp->SetupAttachment(FpsSpringArmComp);
	FpsCameraComp->bUsePawnControlRotation = true;
	FpsCameraComp->bEnableFirstPersonFieldOfView = true;
	FpsCameraComp->FirstPersonFieldOfView = 90.0f;

	//TPS Cam Settings
	TpsSpringArmComp = CreateDefaultSubobject<USpringArmComponent>("TPS Spring Arm");
	TpsSpringArmComp->SetupAttachment(RootComponent);
	TpsSpringArmComp->bUsePawnControlRotation = true;
	TpsSpringArmComp->SetRelativeLocation(FVector(0, 0, 50));
	TpsCameraComp = CreateDefaultSubobject<UCameraComponent>("TPS Cam");
	TpsCameraComp->SetupAttachment(TpsSpringArmComp);
	TpsCameraComp->bUsePawnControlRotation = false;
	TpsCameraComp->SetActive(false);
	

	//FPS WeaponList
	WeaponListSceneComp = CreateDefaultSubobject<USceneComponent>("WeaponList");
	WeaponListSceneComp->SetupAttachment(RootComponent);

	//Basic Init
	JumpMaxCount = 3;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpForce;
	GetCharacterMovement()->GravityScale *= GravityMultiplier;
	GetCharacterMovement()->AirControl = 0.5f;

	//Damage Comp
	HeadBox = CreateDefaultSubobject<UPlayerDamageComponent>("HeadBox");
	HeadBox->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	
	BodyBox = CreateDefaultSubobject<UPlayerDamageComponent>("BodyBox");
	BodyBox->SetupAttachment(GetMesh(), TEXT("BodySocket"));

	//UI
	ConstructorHelpers::FClassFinder<UPlayerUI> playerui(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/WBP/WBP_PlayerUI.WBP_PlayerUI_C'"));
	if (playerui.Succeeded())
		PlayerUIClass = playerui.Class;
	
	//Add Component
	WeaponComp = CreateDefaultSubobject<UWeaponComponent>("WeaponComp");
	//////////////////////////////Input/////////////////////////////////
	{
		ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));
		if (TempIMC.Succeeded())
			IMC_NomPlayer = TempIMC.Object;

		// Input Actions (IA_*)
		ConstructorHelpers::FObjectFinder<UInputAction> FndMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
		if (FndMove.Succeeded())
			IA_Move = FndMove.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
		if (FndJump.Succeeded())
			IA_Jump = FndJump.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
		if (FndLook.Succeeded())
			IA_Look = FndLook.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndRun(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Run.IA_Run'"));
		if (FndRun.Succeeded())
			IA_Run = FndRun.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndCrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Crouch.IA_Crouch'"));
		if (FndCrouch.Succeeded())
			IA_Crouch = FndCrouch.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndInteract(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Interact.IA_Interact'"));
		if (FndInteract.Succeeded())
			IA_Interact = FndInteract.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndFire(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
		if (FndFire.Succeeded())
			IA_Fire = FndFire.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndAim(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Aim.IA_Aim'"));
		if (FndAim.Succeeded())
			IA_Aim = FndAim.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndReload(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Reload.IA_Reload'"));
		if (FndReload.Succeeded())
			IA_Reload = FndReload.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndMelee(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Melee.IA_Melee'"));
		if (FndMelee.Succeeded())
			IA_Melee = FndMelee.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndThrow(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Throw.IA_Throw'"));
		if (FndThrow.Succeeded())
			IA_Throw = FndThrow.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndSkill(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill.IA_Skill'"));
		if (FndSkill.Succeeded())
			IA_Skill = FndSkill.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndUltimate(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Ultimate.IA_Ultimate'"));
		if (FndUltimate.Succeeded())
			IA_Ultimate = FndUltimate.Object;
		ConstructorHelpers::FObjectFinder<UInputAction> FndChangeWeapon(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ChangeWeapon.IA_ChangeWeapon'"));
		if (FndChangeWeapon.Succeeded())
			IA_ChangeWeapon = FndChangeWeapon.Object;
	}
}

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

	TArray<USceneComponent*> ChildComps;
	WeaponListSceneComp->GetChildrenComponents(true, ChildComps);
	for (USceneComponent* childs : ChildComps)
	{	
		if (UChildActorComponent* child = Cast<UChildActorComponent>(childs))
		{
            // 무기 부착 시 소켓 위치/회전에 정확히 스냅되도록 처리
            child->AttachToComponent(
                GetMesh(),
                FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                TEXT("WeaponSocket"));
		}
	}
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
	WeaponComp->OnBulletChangeDelegate.AddDynamic(PlayerUI, &UPlayerUI::UpdateAmmoUI);
	WeaponComp->OnChangeWeaponDelegate.AddDynamic(PlayerUI, &UPlayerUI::UpdateEquipedWeaponUI);
	PlayerUI->AddToViewport();
	PlayerUI->UpdateHealthUI(Hp, MaxHp);
	WeaponComp->Init();

	HeadBox->Init(FVector(10), ECC_EngineTraceChannel1, FName("Head"), EBodyType::Head);
	BodyBox->Init(FVector(50, 15, 20), ECC_EngineTraceChannel2, FName("Body"), EBodyType::Body);

	FTimerHandle SightTimerHandle;
	GetWorldTimerManager().SetTimer(SightTimerHandle, [this]()
	{
		const FVector Start = FpsCameraComp->GetComponentLocation();
		const FVector End = Start + FpsCameraComp->GetForwardVector() * 10000;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(this);
		if (FHitResult Hit; GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params))
		{
			if (const auto Enemy = Cast<AEnemyBase>(Hit.GetActor()))
			{
				Enemy->OnAimByPlayerSight();
			}
		}
	}, 0.1, true);
}

bool ANomPlayer::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() || bIsCrouched;
}

void ANomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PRINTLOG(TEXT("ACTION STATE : %s"), *UEnum::GetValueAsString(ActionState));
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan,
		FString::Printf(TEXT("%s / %s"), *UEnum::GetValueAsString(MovingState), *UEnum::GetValueAsString(ActionState)), false);
}

void ANomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANomPlayer::MoveInput);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ANomPlayer::MoveInput);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANomPlayer::LookInput);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ANomPlayer::JumpInput);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ANomPlayer::RunToggle);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ANomPlayer::CrouchToggle);

		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &ANomPlayer::InteractHold);

		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ANomPlayer::Fire);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ANomPlayer::Fire);
		
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &ANomPlayer::Aim);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &ANomPlayer::Aim);
		
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &ANomPlayer::ReloadStart);
		
		EnhancedInputComponent->BindAction(IA_Melee, ETriggerEvent::Started, this, &ANomPlayer::Melee);
		EnhancedInputComponent->BindAction(IA_Throw, ETriggerEvent::Started, this, &ANomPlayer::Throw);
		EnhancedInputComponent->BindAction(IA_Skill, ETriggerEvent::Started, this, &ANomPlayer::Skill);
		EnhancedInputComponent->BindAction(IA_Ultimate, ETriggerEvent::Started, this, &ANomPlayer::UltimateSkill);

		EnhancedInputComponent->BindAction(IA_ChangeWeapon, ETriggerEvent::Started, this, &ANomPlayer::ChangeWeapon);
	}
}

void ANomPlayer::MoveInput(const FInputActionValue& Value)
{
	if (bIsDead) return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector == FVector2D::ZeroVector)
	{
		MovingState = EMovingState::Idle;
	}
	
	MoveFunc(MovementVector.X, MovementVector.Y);
}

void ANomPlayer::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookFunc(LookAxisVector.X, LookAxisVector.Y);
}

void ANomPlayer::JumpInput()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill)
		return;
	
	if (MovingState == EMovingState::Crouch)
	{
		UnCrouch();
	}
	
	Jump();
	MovingState = (MovingState == EMovingState::Running ? EMovingState::Running : EMovingState::Idle);
}

void ANomPlayer::RunToggle()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand)
		return;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	
	if (MovingState == EMovingState::Crouch)
		UnCrouch();

	ActionState = EActionState::Idle;
	MovingState = MovingState == EMovingState::Running ? EMovingState::Idle : EMovingState::Running;
	PRINTINFO();
}

void ANomPlayer::CrouchToggle()
{
	if (bIsDead) return;
	
	if (GetMovementComponent()->IsFalling() || ActionState == EActionState::Skill)
	{
		return;
	}
	
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;
	
	if (MovingState == EMovingState::Crouch)
		UnCrouch();
	else
		Crouch();
	
	MovingState = (MovingState == EMovingState::Crouch ? EMovingState::Idle : EMovingState::Crouch);
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
		if (bIsAiming)
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed * 0.85;
		else if (MovingState == EMovingState::Running)
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed * 2;
		else
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
		
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
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
	if (bIsDead) return;
	
	if (Value.Get<bool>())
	{
		bIsHoldFire = true;
		
		if (WeaponComp->GetCurrentWeapon()->CanFire() == false)
		{
			if (WeaponComp->GetCurrentWeapon()->CanReload())
				ReloadStart();
			else
				return;
		}

		if (ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand
			|| ActionState == EActionState::Skill || ActionState == EActionState::Reloading)
			return;
	
		if (MovingState == EMovingState::Running)
			MovingState = EMovingState::Idle;

		WeaponComp->FireStart();
		ActionState = EActionState::Firing;
	}
	else
	{
		WeaponComp->FireEnd();
		ActionState = (ActionState == EActionState::Firing ? EActionState::Idle : ActionState);
		bIsHoldFire = false;
	}
}

void ANomPlayer::Aim(const FInputActionValue& Value)
{
	if (bIsDead) return;
	
	if (Value.Get<bool>())
	{
		bIsHoldAim = true;
		
		if (WeaponComp->GetCurrentWeapon()->GetData()->IsAimable == false)
		{
			bIsHoldAim = false;
			return;
		}
		
		if (ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand
			|| ActionState == EActionState::Skill || ActionState == EActionState::Reloading)
			return;

		if (MovingState == EMovingState::Running)
			MovingState = EMovingState::Idle;
		
		bIsAiming = true;
		WeaponComp->AimStart();
	}
	else
	{
		bIsAiming = false;
		WeaponComp->AimEnd();
		bIsHoldAim = false;
	}
}

void ANomPlayer::ReloadStart()
{
	if (bIsDead) return;
	
	if (WeaponComp->GetCurrentWeapon()->CanReload() == false)
		return;
	
	if (ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand
		|| ActionState == EActionState::Skill || ActionState == EActionState::Reloading)
		return;
	
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}


	ActionState = EActionState::Reloading;
	
	//TODO : 시간이 지나면 리로딩 성공 이벤트 설정 (몽타쥬)- 현재는 타임라인으로
	GetWorldTimerManager().SetTimer(ReloadHandle, [this]()
	{
		ReloadEnd();
	}, WeaponComp->GetCurrentWeapon()->GetData()->ReloadDuration, false);
}

void ANomPlayer::ReloadEnd()
{
	WeaponComp->Reload();
	if (ActionState == EActionState::Reloading)
		ActionState = EActionState::Idle;

	if (bIsHoldFire)
	{
		ActionState = EActionState::Firing;
		WeaponComp->FireStart();
		bIsHoldFire = false;
		
	}
	if (bIsHoldAim)
	{
		bIsAiming = true;
		bIsHoldAim = false;
		WeaponComp->AimStart();
	}
}

void ANomPlayer::Melee()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand)
		return;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;

	ActionState = EActionState::LeftHand;

	
	EObjectTypeQuery ObjType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<AActor*> ActorsToIgnore = {this};
	TArray<FHitResult> OutHits;
	FVector Offset = FpsCameraComp->GetForwardVector() * 60.f
			   + FpsCameraComp->GetRightVector() * -15.f
			   + FpsCameraComp->GetUpVector() * 40.f;

	FVector Start = GetActorLocation() + Offset;
	FVector End   = GetActorLocation() + Offset + FpsCameraComp->GetForwardVector() * 40.f;
	if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Start, End, FVector(32,32,50),
		FpsCameraComp->GetComponentRotation(), TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType}, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,OutHits,
		true, FColor::Red, FColor::Green, 10))
	{
		for (FHitResult& hits : OutHits)
		{
			if (UDamageComponent* act = Cast<UDamageComponent>(hits.GetComponent()))
			{
				act->OnDamaged(FFireInfo(FistDamage, GetActorLocation(), ETeamInfo::Player, false));
			}
		}
	}
	
	//TODO : 몽타쥬로 바꿀 것
	PRINTINFO();
	GetWorldTimerManager().SetTimer(LeftHandHandle, [this]()
	{
		LeftHandEnd();
	}, 1.f, false);
}

void ANomPlayer::Throw()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand)
		return;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;

	ActionState = EActionState::LeftHand;
	//TODO : 몽타쥬로 바꿀 것
	GetWorld()->SpawnActor<AGrenade>(AGrenade::StaticClass()
		, GetFpsCam()->GetComponentLocation() + GetFpsCam()->GetRightVector() * -30
		, FRotator(0))
		->Init(GetFpsCam()->GetForwardVector(), 1000 + GetVelocity().X);
	PRINTINFO();
	GetWorldTimerManager().SetTimer(LeftHandHandle, [this]()
	{
		LeftHandEnd();
	}, 1.f, false);
}

void ANomPlayer::LeftHandEnd()
{
	ActionState = EActionState::Idle;
	if (bIsHoldFire)
	{
		ActionState = EActionState::Firing;
		bIsHoldFire = false;
		WeaponComp->FireStart();
	}
	if (bIsHoldAim)
	{
		bIsAiming = true;
		bIsHoldAim = false;
		WeaponComp->AimStart();
	}
}

void ANomPlayer::Skill()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand)
		return;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	if (MovingState == EMovingState::Crouch)
	{
		MovingState = EMovingState::Idle;
		UnCrouch();
	}
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;

	ChangeToTps();

	//TODO : 몽타쥬로 바꿀 것
	PRINTINFO();
	GetWorldTimerManager().SetTimer(SkillHandle, [this]()
	{
		SkillEnd();
	}, 1.f, false);
}

void ANomPlayer::SkillEnd()
{
	ActionState = EActionState::Idle;
	ChangeToFps();
	if (bIsHoldFire)
	{
		ActionState = EActionState::Firing;
		bIsHoldFire = false;
		WeaponComp->FireStart();
	}
	if (bIsHoldAim)
	{
		bIsAiming = true;
		bIsHoldAim = false;
		WeaponComp->AimStart();
	}
}

void ANomPlayer::UltimateSkill()
{
	if (bIsDead) return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::ChangeWeapon || ActionState == EActionState::LeftHand)
		return;
	if (ActionState == EActionState::Firing)
	{
		OnFireCanceled();
	}
	if (bIsAiming)
	{
		OnAimCanceled();
	}
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	if (MovingState == EMovingState::Crouch)
	{
		MovingState = EMovingState::Idle;
		UnCrouch();
	}
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;

	ChangeToTps();
	PRINTINFO();
	//TODO : 몽타쥬로 바꿀 것
	GetWorldTimerManager().SetTimer(SkillHandle, [this]()
	{
		UltimateSkillEnd();
	}, 1.f, false);
}

void ANomPlayer::UltimateSkillEnd()
{
	ActionState = EActionState::Idle;
	ChangeToFps();
	if (bIsHoldFire)
	{
		ActionState = EActionState::Firing;
		bIsHoldFire = false;
		WeaponComp->FireStart();
	}
	if (bIsHoldAim)
	{
		bIsAiming = true;
		bIsHoldAim = false;
		WeaponComp->AimStart();
	}
}

void ANomPlayer::ChangeWeapon(const FInputActionValue& Value)
{
	if (bIsDead) return;
	
	int32 WeaponIndex = static_cast<int32>(Value.Get<float>());
	if (WeaponComp->GetCurrentWeaponIdx() == WeaponIndex - 1)
		return;
	
	if (ActionState == EActionState::Skill || ActionState == EActionState::LeftHand || ActionState == EActionState::ChangeWeapon)
		return;
	if (ActionState == EActionState::Reloading)
	{
		OnReloadCanceled();
	}
	if (ActionState == EActionState::Firing)
		OnFireCanceled();
	if (bIsAiming)
		OnAimCanceled();
	
	if (MovingState == EMovingState::Running)
		MovingState = EMovingState::Idle;

	ActionState = EActionState::ChangeWeapon;

	GetWorldTimerManager().SetTimer(PutWeaponHandle, [this, WeaponIndex]()
	{
		WeaponComp->ChangeWeapon(WeaponIndex - 1);
	}, WeaponComp->GetCurrentWeapon()->GetData()->AimDuration, false);
	
	//TODO : 몽타쥬로 바꿀것
	GetWorldTimerManager().SetTimer(ChangeWeaponHandle, [this, WeaponIndex]()
	{
		OnWeaponChanged(WeaponIndex - 1);
	}, WeaponComp->GetCurrentWeapon()->GetData()->EquipDuration, false);

	
}

void ANomPlayer::OnWeaponChanged(float Idx)
{
	WeaponComp->OnWeaponChanged(Idx);
	ActionState = EActionState::Idle;
	if (bIsHoldFire)
	{
		ActionState = EActionState::Firing;
		bIsHoldFire = false;
		WeaponComp->FireStart();
	}
	if (bIsHoldAim)
	{
		bIsAiming = true;
		bIsHoldAim = false;
		WeaponComp->AimStart();
	}
}

void ANomPlayer::OnReloadCanceled()
{
	ActionState = EActionState::Idle;
	if (GetWorldTimerManager().IsTimerActive(ReloadHandle))
		GetWorldTimerManager().ClearTimer(ReloadHandle);
}

void ANomPlayer::OnFireCanceled()
{
	ActionState = EActionState::Idle;
	WeaponComp->FireEnd();
}

void ANomPlayer::OnAimCanceled()
{
	bIsAiming = false;
	WeaponComp->AimEnd();
}

void ANomPlayer::ChangeToFps()
{
	TpsMeshComp->SetOwnerNoSee(true);
	GetMesh()->SetOwnerNoSee(false);
	FpsCameraComp->SetActive(true);
	TpsCameraComp->SetActive(false);
}

void ANomPlayer::ChangeToTps()
{
	ActionState = EActionState::Skill;
	
	TpsMeshComp->SetOwnerNoSee(false);
	GetMesh()->SetOwnerNoSee(true);
	FpsCameraComp->SetActive(false);
	TpsCameraComp->SetActive(true);
}

//여기에 데미지 함수 구현
void ANomPlayer::OnDamaged(FFireInfo Info)
{
	if (Info.TeamInfo == ETeamInfo::Player)
		Hp -= Info.Damage / 100;
	else
		Hp -= Info.Damage;
	
	PlayerUI->UpdateHealthUI(Hp, MaxHp);

	if (Hp <= 0)
	{
		
	}
}

//여기에 크리티컬 데미지 함수 구현
void ANomPlayer::OnCriticalDamaged(FFireInfo Info)
{
	if (Info.TeamInfo == ETeamInfo::Player)
		Hp -= Info.Damage * 2 / 100;
	else
		Hp -= Info.Damage;
	
	PlayerUI->UpdateHealthUI(Hp, MaxHp);

	if (Hp <= 0)
	{
		bIsDead = true;
		ChangeToTps();
		
		ActionState = EActionState::Idle;
		MovingState = EMovingState::Idle;
		bIsAiming = false;
		bIsHoldFire = false;
		bIsHoldAim = false;
		OnFireCanceled();
		OnReloadCanceled();
		OnAimCanceled();
		UnCrouch();

		TpsMeshComp->SetSimulatePhysics(true);
		TpsMeshComp->WakeAllRigidBodies();
	}
}

USpringArmComponent* ANomPlayer::GetFpsCamArm()
{
	return FpsSpringArmComp;
}

UCameraComponent* ANomPlayer::GetFpsCam()
{
	return FpsCameraComp;
}

const EActionState& ANomPlayer::GetActionState() const
{
	return ActionState;
}

const EMovingState& ANomPlayer::GetMovingState() const
{
	return MovingState;
}

const bool& ANomPlayer::GetIsAiming() const
{
	return bIsAiming;
}
