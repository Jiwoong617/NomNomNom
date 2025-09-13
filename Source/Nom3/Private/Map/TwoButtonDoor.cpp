 // Fill out your copyright notice in the Description page of Project Settings.

  #include "Map/TwoButtonDoor.h"
  #include "Map/InteractiveButton.h"

  ATwoButtonDoor::ATwoButtonDoor()
  {
      PrimaryActorTick.bCanEverTick = true;

      bShouldBeOpen = false;
      bIsButton1Active = false;
      bIsButton2Active = false;
  }

  void ATwoButtonDoor::BeginPlay()
  {
      Super::BeginPlay();

      // 시작 위치와 목표 위치 설정 (AutoDoor 로직과 동일)
      StartLocation = GetActorLocation();
      GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

      if (StartLocation.IsNearlyZero())
      {
          StartLocation = GetActorLocation();
      }
      if (GlobalTargetLocation.IsNearlyZero())
      {
          GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
      }

      MaxSpeed = ObjectSpeed;
      ObjectSpeed = 0.0f;
  }

  void ATwoButtonDoor::Tick(float DeltaTime)
  {
      Super::Tick(DeltaTime);

      //auto door 랑 비슷하게
      FVector Destination = bShouldBeOpen ? GlobalTargetLocation : StartLocation;
      float DistanceToDestination = FVector::Dist(GetActorLocation(), Destination);

      float TargetSpeed = (DistanceToDestination > 1.0f) ? MaxSpeed : 0.0f;

      ObjectSpeed = FMath::FInterpTo(ObjectSpeed, TargetSpeed, DeltaTime, Acceleration);

      if (FMath::IsNearlyZero(ObjectSpeed) && FMath::IsNearlyZero(TargetSpeed))
      {
          if (DistanceToDestination > 0.0f)
          {
              SetActorLocation(Destination);
          }
          ObjectSpeed = 0.0f;
          return;
      }

      FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
      FVector Movement = Direction * ObjectSpeed * DeltaTime;

      if (Movement.Size() >= DistanceToDestination)
      {
          SetActorLocation(Destination);
          ObjectSpeed = 0.0f;
      }
      else
      {
          SetActorLocation(GetActorLocation() + Movement);
      }
  }

  void ATwoButtonDoor::ActivateButton(AInteractiveButton* Button)
  {
      if (Button == Button1)
      {
          bIsButton1Active = true;
          UE_LOG(LogTemp, Warning, TEXT("Button 1 Activated"));
      }
      else if (Button == Button2)
      {
          bIsButton2Active = true;
          UE_LOG(LogTemp, Warning, TEXT("Button 2 Activated"));
      }
      CheckDoorState();
  }

  void ATwoButtonDoor::DeactivateButton(AInteractiveButton* Button)
  {
      if (Button == Button1)
      {
          bIsButton1Active = false;
          UE_LOG(LogTemp, Warning, TEXT("Button 1 Deactivated"));
      }
      else if (Button == Button2)
      {
          bIsButton2Active = false;
          UE_LOG(LogTemp, Warning, TEXT("Button 2 Deactivated"));
      }
      CheckDoorState();
  }
    // 여기서 결정
  void ATwoButtonDoor::CheckDoorState()
  {
      if (bIsButton1Active && bIsButton2Active)
      {
          bShouldBeOpen = true;
          UE_LOG(LogTemp, Warning, TEXT("Both buttons are active. Opening door."));
      }
      else
      {
          bShouldBeOpen = false;
          UE_LOG(LogTemp, Warning, TEXT("A button is not active. Closing door."));
      }
  }
  