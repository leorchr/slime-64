// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Main.h"

#include "Camera/CameraComponent.h"
#include "Character/CustomCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACharacter_Main::ACharacter_Main(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm= ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, "CameraBoom");
	if (SpringArm)
	{
		if (!GetMesh()) return;
		SpringArm->SetupAttachment(GetMesh());
	}
	Camera = ObjectInitializer.CreateDefaultSubobject<UCustomCameraComponent>(this, "Camera");
	if (Camera)
	{
		if (!SpringArm) return;
		Camera->SetupAttachment(SpringArm);
	}
}

// Called when the game starts or when spawned
void ACharacter_Main::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Main::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

