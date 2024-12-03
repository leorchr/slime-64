// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCameraComponent.h"

#include "InputActionValue.h"
#include "Character/Character_Main.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/SpringArmComponent.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomCameraComponent::Rotate(const FInputActionValue& Value)
{
	if (!BoomStick) return;
	FVector2d Axis = Value.Get<FVector2d>();

	Character->AddControllerYawInput(Axis.X);
	Character->AddControllerPitchInput(-Axis.Y);
}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!BoomStick) return;

	BoomStick->SetWorldLocation(FMath::VInterpTo(BoomStick->GetComponentLocation(), Character->GetActorLocation(), DeltaTime, PositionLag));
}

FVector UCustomCameraComponent::GetPredictLocation()
{
	if (!Character) return FVector(0., 0., 0.);
	FVector Location = Character->GetActorLocation();
	FVector Velocity = Character->GetVelocity();
	return FVector(0.0f,0.0f,0.0f);
}

void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	bUsePawnControlRotation = false;

	Character = Cast<ACharacter_Main>(GetOwner());
	if (!Character) return;

	BoomStick = Character->GetComponentByClass<USpringArmComponent>();
	if (!BoomStick) return;

	BoomStick->SetUsingAbsoluteLocation(true);
	BoomStick->bUsePawnControlRotation = true;
	BoomStick->bInheritRoll = false;
	BoomStick->bEnableCameraRotationLag = true;
	BoomStick->CameraRotationLagSpeed = RotationLag;
}
