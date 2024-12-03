// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Controller_Main.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Character/Character_Main.h"

void AController_Main::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!inputSubsystem)
		return;
	inputSubsystem->ClearAllMappings();
	inputSubsystem->AddMappingContext(inputMapping, 0);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!inputComponent)
		return;
	inputComponent->ClearActionBindings();
	inputComponent->BindAction(inputActionMove, ETriggerEvent::Triggered, this, &AController_Main::MovePlayer);
	inputComponent->BindAction(inputActionRun, ETriggerEvent::Triggered, this, &AController_Main::Run);
	inputComponent->BindAction(inputActionJump, ETriggerEvent::Triggered, this, &AController_Main::Jump);
}

void AController_Main::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (character)
		return;

	character = Cast<ACharacter_Main>(InPawn);

	// Get reference to our Character
	CameraController = GetComponentByClass<UCustomCameraController>();
	
	if (CameraController.IsValid())
	{
		CameraController->SetupInputComponent(InputComponent, Cast<ACharacter_Main>(InPawn));
	}
}

void AController_Main::MovePlayer(const FInputActionValue& value)
{
	if (!character)
		return;

	const FVector2D moveValue = value.Get<FVector2D>();

	if (moveValue.Y != 0.f) {
		const FVector3d dir = character->GetActorForwardVector() * moveValue.Y;
		character->AddMovementInput(dir);
	}

	if (moveValue.X != 0.f) {
		const FVector3d dir = character->GetActorRightVector() * moveValue.X;
		character->AddMovementInput(dir);
	}
}

void AController_Main::Jump(const FInputActionValue& value)
{
	character->Jump();
}

void AController_Main::Run(const FInputActionValue& value)
{
	//Run Script
}
