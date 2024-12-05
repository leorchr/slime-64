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

	if (Character->bIsRunning)
	{
		PositionLag = PositionRunningLag;
		SetFieldOfView(FMath::Lerp(FieldOfView, RunningFOV, FOVLerp));
		BoomStick->TargetArmLength = FMath::Lerp(BoomStick->TargetArmLength, RunningSpringLength, SpringLengthLerp);
	} else {
		PositionLag = PositionWalkingLag;
		SetFieldOfView(FMath::Lerp(FieldOfView, WalkingFOV, FOVLerp));
		BoomStick->TargetArmLength = FMath::Lerp(BoomStick->TargetArmLength, WalkingSpringLength, SpringLengthLerp);
	}
	
	if (Character->movement->IsFalling())
	{
		PositionLag = PositionJumpingLag;
		BoomStick->SetWorldLocation(FMath::VInterpTo(BoomStick->GetComponentLocation(), Character->GetActorLocation(), DeltaTime, PositionLag));
		Character->AddControllerPitchInput(GetPitch(DeltaTime) * PitchLag);
	} else
	{
		if (Offset < EndZone)
		{
			Timer += DeltaTime;
			if (Timer > TimeInZone)
			{
				SeekPlayer = false;
				Timer = 0.0f;
				return;
			}
		}
		if (Offset > StartZone)
		{
			SeekPlayer = true;
			Timer = 0.0f;
		}

		if (SeekPlayer)
		{
			BoomStick->SetWorldLocation(FMath::VInterpTo(BoomStick->GetComponentLocation(), GetPredictLocation(), DeltaTime, PositionLag));
		}
	}
	
	Character->AddControllerYawInput(GetYaw(DeltaTime) * YawLag);
}

FVector UCustomCameraComponent::GetPredictLocation()
{
	if (!Character) return FVector(0., 0., 0.);
	FVector Location = Character->GetActorLocation();
	FVector Velocity = Character->GetVelocity();

	float Delta = ForwardDelta;
	Delta *= 1.0f - FMath::Clamp(FVector::DotProduct(Character->ActorForward.GetSafeNormal(), Velocity.GetSafeNormal()), 0.0, 1.0);
	
	Velocity *= Delta;
	Location += Velocity;
	
	return Location;
}

double UCustomCameraComponent::GetYaw(float DeltaTime)
{
	FRotator RequiredRotator = FQuat::FindBetweenVectors(GetForwardVector(), Character->GetVelocity().GetSafeNormal()).Rotator();
	float RotationStrength = 0.0001 + FMath::Abs(FVector::DotProduct(GetRightVector().GetSafeNormal(), Character->GetVelocity().GetSafeNormal()));
	RotationStrength *= Character->GetVelocity().Length() / Character->movement->MaxWalkSpeed;
	FRotator Rotation = FMath::RInterpTo(FRotator::ZeroRotator, RequiredRotator, DeltaTime, RotationStrength);
	return Rotation.Yaw;
}

double UCustomCameraComponent::GetPitch(float DeltaTime)
{
	FRotator RequiredRotator = FQuat::FindBetweenVectors(GetForwardVector(), Character->GetVelocity().GetSafeNormal()).Rotator();

	float RotationStrength = 0.0001 + FMath::Abs(FVector::DotProduct(-GetUpVector().GetSafeNormal(), Character->GetVelocity().GetSafeNormal()));
	RotationStrength *= Character->GetVelocity().Length() / Character->movement->MaxWalkSpeed;
	FRotator Rotation = FMath::RInterpTo(FRotator::ZeroRotator, RequiredRotator, DeltaTime, RotationStrength);
	return (Character->GetVelocity().Z * Rotation.Pitch > 0) ? -Rotation.Pitch : Rotation.Pitch;
}

void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BoomStick && Character)
		BoomStick->SetWorldLocation(Character->GetActorLocation());
	
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
