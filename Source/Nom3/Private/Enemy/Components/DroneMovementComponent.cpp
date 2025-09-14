// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Components/DroneMovementComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UDroneMovementComponent::UDroneMovementComponent() :
	bSimulate(false),
	GravityForce(0),
	GravityDir(FVector::DownVector),
	CruiseThrustForce(2500),
	EvadeThrustLevel(30),
	ThrustForce(0),
	ThrustDir(FVector::ZeroVector),
	ThrustVectoringLevel(0.75),
	LaunchSpeed(1000),
	SplashSpeed(500),
	Bounciness(0.25),
	DragCoefficient(4.0),
	Mass(10)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//제트 펄스 오디오 컴포넌트
	JetPulseAudioComp = CreateDefaultSubobject<UAudioComponent>("JetPulseAudioComp");
	JetPulseAudioComp->bAutoActivate = true;
	
	//제트 펄스 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/ScoutShank/Sound/SW_JetPulse.SW_JetPulse"));
		Finder.Succeeded())
	{
		JetPulseSound = Finder.Object;
		JetPulseAudioComp->SetSound(JetPulseSound);
	}

	 //제트 펄스 어테뉴에이션 로드
	 if (static ConstructorHelpers::FObjectFinder<USoundAttenuation> Finder(
	 	TEXT("/Game/Asset/ScoutShank/Sound/SA_ScoutShankFireAttenuation.SA_ScoutShankFireAttenuation"));
	 	Finder.Succeeded())
	 {
	 	JetPulseAudioComp->SetAttenuationSettings(Finder.Object);
	 }

	//폭발 사운드 로드
	if (static ConstructorHelpers::FObjectFinder<USoundBase> Finder(
		TEXT("/Game/Asset/ScoutShank/Sound/SC_ScoutShankExplosionCue.SC_ScoutShankExplosionCue"));
		Finder.Succeeded())
	{
		ExplosionSound = Finder.Object;
	}
}

void UDroneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//부모에게 부착
	JetPulseAudioComp->SetupAttachment(GetOwner()->GetRootComponent());
}

void UDroneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//시뮬레이션 활성화 여부
	if (bSimulate == false)
	{
		return;
	}

	//중력 적용 (등가 원리)
	Velocity += GravityForce * GravityDir * DeltaTime;

	//추진력 적용
	Velocity += (ThrustForce * ThrustDir / Mass) * DeltaTime;
	
	//공기 저항 적용
	const FVector DragForce = -Velocity * DragCoefficient;
	const FVector DragAcceleration = DragForce / Mass;
	Velocity += DragAcceleration * DeltaTime;
	
	//이동량 연산
	const FVector Delta = Velocity * DeltaTime;

	//속도의 방향을 드로우
	const FVector Location = GetOwner()->GetActorLocation();
	
	DrawDebugLine(GetWorld(), Location, Location + Velocity, FColor::Red, false, -1, 0, 0 );

	//DrawDebugLine(GetWorld(), Location, Location + ThrustDir * 500, FColor::Green, false, -1, 0, 1 );
	
	//안전 이동 이동
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true, Hit);

	//충돌 발생
	if (Hit.bBlockingHit)
	{
		//튕겨나감
		Velocity = Velocity.MirrorByVector(Hit.ImpactNormal) * Bounciness;
	}
}

void UDroneMovementComponent::Launch(const FVector LaunchDir)
{
	//사출 속도 결정
	Velocity = LaunchDir * LaunchSpeed;

	//추진 방향 초기화
	ThrustDir = LaunchDir;

	//시뮬레이션 활성화
	bSimulate = true;
}

void UDroneMovementComponent::Splash(const FVector SplashDir)
{
	//충격 방향
	const FVector ImpulseDir = UKismetMathLibrary::ProjectVectorOnToPlane(SplashDir, GravityDir).GetSafeNormal();

	//충격 방향 랜덤화
	const FVector RandomImpulseDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ImpulseDir, 45);
	
	//충격 연산
	const FVector Impulse = RandomImpulseDir * SplashSpeed;

	//가속
	Velocity += Impulse / Mass;
}

void UDroneMovementComponent::ThrottleThrustByLevel(const float Level)
{
	//목표 추진력 결정
	ThrustForce = CruiseThrustForce * Level;
}

void UDroneMovementComponent::ThrottleToCruise()
{
	//순항 추진력을 향해 점진적 보간
	ThrustForce = FMath::Lerp(ThrustForce, CruiseThrustForce, 0.1);
}

void UDroneMovementComponent::ThrottleToHighManeuver(const float Level)
{
	//기존 속도 급감
	Velocity = FMath::Lerp(Velocity, FVector::ZeroVector, 0.8);
	
	//추진력 급상승
	ThrottleThrustByLevel(Level);
}

void UDroneMovementComponent::ThrottleOff()
{
	//추진력을 0으로 초기화
	ThrustForce = 0;
}

void UDroneMovementComponent::ReverseVectorThrust(const float Accel)
{
	if (Velocity.IsNearlyZero())
	{
		return;
	}
	
	//추진 방향 반대
	ThrustDir = -Velocity.GetSafeNormal();

	//추진력 조절
	ThrustForce = Accel * Mass;
}

void UDroneMovementComponent::VectorThrust(const FVector VectorDir)
{
	//추진 방향 구면 보간
	ThrustDir = VectorDir.GetSafeNormal();
}

void UDroneMovementComponent::Fall()
{
	//추진력 상실
	ThrustForce = 0;

	//중력 가속 시작
	GravityForce = 980;

	//제트 펄스 오디오 컴포넌트 비활성화
	JetPulseAudioComp->Deactivate();

	//폭발 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetOwner()->GetActorLocation());
}