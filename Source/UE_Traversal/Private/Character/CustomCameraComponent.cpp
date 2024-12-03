// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCameraComponent.h"

#include "Character/Character_Main.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomCameraComponent::Update()
{
}

void UCustomCameraComponent::Rotate(const FInputActionValue& axis)
{
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
