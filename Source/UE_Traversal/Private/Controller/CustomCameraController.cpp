// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CustomCameraController.h"

#include "Character/Character_Main.h"

// Sets default values for this component's properties
UCustomCameraController::UCustomCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCustomCameraController::SetupInputComponent(TObjectPtr<UInputComponent> InputComponent, ACharacter_Main* Character)
{
}

// Called when the game starts
void UCustomCameraController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

