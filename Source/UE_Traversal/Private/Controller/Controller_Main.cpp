// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Controller_Main.h"

#include "Character/Character_Main.h"

void AController_Main::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AController_Main::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// Get reference to our Character
	CameraController = GetComponentByClass<UCustomCameraController>();
	
	if (CameraController.IsValid())
	{
		CameraController->SetupInputComponent(InputComponent, Cast<ACharacter_Main>(InPawn));
	}
}
