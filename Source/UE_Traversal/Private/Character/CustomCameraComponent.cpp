// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCameraComponent.h"

#include "InputActionValue.h"
#include "Character/Character_Main.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameTime.h"
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
	Character->AddControllerPitchInput(Axis.Y);
}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!BoomStick) return;

	float Offset = FVector::Dist(BoomStick->GetComponentLocation(), Character->GetActorLocation());
	if (Character->movement->IsFalling())
	{
		BoomStick->SetWorldLocation(FMath::VInterpTo(BoomStick->GetComponentLocation(), Character->GetActorLocation(), DeltaTime, PositionLag));
	} else
	{
		if (Offset < EndZone)
		{
			SeekPlayer = false;
			return;
		}
		if (Offset > StartZone)
		{
			SeekPlayer = true;
		}

		if (SeekPlayer)
		{
			BoomStick->SetWorldLocation(FMath::VInterpTo(BoomStick->GetComponentLocation(), GetPredictLocation(), DeltaTime, PositionLag));
			Character->AddControllerYawInput(GetRotation(DeltaTime).Yaw);
		}
	}
}

FVector UCustomCameraComponent::GetPredictLocation()
{
	if (!Character) return FVector(0., 0., 0.);
	FVector Location = Character->GetActorLocation();
	FVector Velocity = Character->GetVelocity();

	float Delta = ForwardDelta;
	Delta *= 1.0f - FMath::Clamp(FVector::DotProduct(Character->GetActorForwardVector().GetSafeNormal(), Velocity.GetSafeNormal()), 0.0, 1.0);
	
	Velocity *= Delta;
	Location += Velocity;
	
	return Location;
}

FRotator UCustomCameraComponent::GetRotation(float DeltaTime)
{
	
	FRotator RequiredRotator = FQuat::FindBetweenVectors(GetForwardVector(), Character->GetVelocity().GetSafeNormal()).Rotator();
	float RotationStrength = 0.0001 + OrientationLag * FMath::Abs(FVector::DotProduct(GetRightVector().GetSafeNormal(), Character->GetVelocity().GetSafeNormal()));
	RotationStrength *= Character->GetVelocity().Length() / Character->movement->MaxWalkSpeed;
	FRotator Rotation = FMath::RInterpTo(FRotator::ZeroRotator, RequiredRotator, DeltaTime, RotationStrength);
	return Rotation;
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
	BoomStick->CameraRotationLagSpeed = RotationInputLag;
}
