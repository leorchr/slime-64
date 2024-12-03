// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCameraComponent.h"

#include "InputActionValue.h"
#include "Character/Character_Main.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomCameraComponent::Update()
{
}

void UCustomCameraComponent::Rotate(const FInputActionValue& Value)
{
	FVector2d Axis = Value.Get<FVector2d>();

	Character->AddControllerYawInput(Axis.X);
	Character->AddControllerPitchInput(Axis.Y);
}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter_Main>(GetOwner());
	if (!Character) return;
}
