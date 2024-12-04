// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Main.h"

#include "Camera/CameraComponent.h"
#include "Character/CustomCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacter_Main::ACharacter_Main(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	movement = GetCharacterMovement();

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
	movement->MaxWalkSpeed = MaxWalkSpeed;
	movement->MaxAcceleration = MaxWalkAcceleration;
	movement->JumpZVelocity = JumpForce;

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



void ACharacter_Main::OnLanded(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Has Lnded"));
}

bool ACharacter_Main::CanJumpInternal_Implementation() const
{
	bool groundResult = Super::CanJumpInternal_Implementation();



	return groundResult;
}

void ACharacter_Main::Move(FVector2d Direction)
{
	if (Direction.Y != 0.f) {
		FVector3d cameraForward = Camera->GetForwardVector();
		const FVector3d dir = cameraForward * Direction.Y;
		AddMovementInput(dir);
	}
	if (Direction.X != 0.f) {
		FVector3d cameraRight = Camera->GetRightVector();
		const FVector3d dir = cameraRight * Direction.X;
		AddMovementInput(dir);
	}
	//Stopped Giving Movement
	if (Direction.Length() == 0) {

	}
}

void ACharacter_Main::Run(bool RunToggle)
{
	
	if (RunToggle) {
		movement->MaxWalkSpeed = MaxRunSpeed;
		movement->MaxAcceleration = MaxRunAcceleration;
	}
	else {
		movement->MaxWalkSpeed = MaxWalkSpeed;
		movement->MaxAcceleration = MaxWalkAcceleration;
	}
}
