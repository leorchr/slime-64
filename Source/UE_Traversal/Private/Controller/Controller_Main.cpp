// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Controller_Main.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/CustomCameraComponent.h"
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
	inputComponent->BindAction(inputActionRun, ETriggerEvent::Started, this, &AController_Main::Run);
	inputComponent->BindAction(inputActionRun, ETriggerEvent::Completed, this, &AController_Main::Run);
	inputComponent->BindAction(inputActionRun, ETriggerEvent::Canceled, this, &AController_Main::Run);
	inputComponent->BindAction(inputActionJump, ETriggerEvent::Started, this, &AController_Main::Jump);
	inputComponent->BindAction(inputActionLook, ETriggerEvent::Triggered, this, &AController_Main::Rotate);
}

void AController_Main::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (character)
		return;

	character = Cast<ACharacter_Main>(InPawn);
}

void AController_Main::MovePlayer(const FInputActionValue& value)
{
	if (!character)
		return;

	const FVector2D moveValue = value.Get<FVector2D>();
	character->Move(moveValue);
}

void AController_Main::Jump(const FInputActionValue& value)
{
	if (!character)
		return;

	character->Jump();
}

void AController_Main::Run(const FInputActionValue& value)
{
	//Run Script
	const bool runValue = value.Get<bool>();

	if (!character)
		return;

	UE_LOG(LogTemp, Log, TEXT("IM RUNNING"));
	character->Run(runValue);

	
}

void AController_Main::Rotate(const FInputActionValue& value)
{
	if (character->Camera)
	{
		character->Camera->Rotate(value);
	}
}
