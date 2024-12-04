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



void ACharacter_Main::setNewState(EMovementState newState)
{
	OnStateChange.Broadcast(currentState, newState);
	currentState = newState;
	const FString Message = UEnum::GetValueAsString(newState);
	UE_LOG(LogTemp,Log,TEXT("New State : %s"),*Message)
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


void ACharacter_Main::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCounter = JumpNumber;
	if (movement->Velocity.Length() == 0) {
		setNewState(EMovementState::Idle);
	}
	else {
		setNewState(bIsRunning ? EMovementState::Running : EMovementState::Walking);
	}
}

void ACharacter_Main::OnJumped_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Jumpes"))
	Super::OnJumped_Implementation();
	JumpCounter--;
	setNewState(EMovementState::Jumping);
}


bool ACharacter_Main::CanJumpInternal_Implementation() const
{
	bool bGroundResult = Super::CanJumpInternal_Implementation();

	if (!bGroundResult) 
	{
		return JumpCounter > 0;
	}


	return true;
}

void ACharacter_Main::NotifyJumpApex()
{
}

void ACharacter_Main::Move(FVector2d Direction)
{
	if (Direction.Y != 0.f) {
		FVector3d cameraForward = Camera->GetForwardVector();
		cameraForward.Z = 0;
		cameraForward.Normalize();
		const FVector3d dir = cameraForward * Direction.Y;
		AddMovementInput(dir);

		
	}
	if (Direction.X != 0.f) {
		FVector3d cameraRight = Camera->GetRightVector();
		cameraRight.Z = 0;
		cameraRight.Normalize();
		const FVector3d dir = cameraRight * Direction.X;
		AddMovementInput(dir);
	}



	//Stopped Giving Movement
	if (Direction.Length() == 0) {
		if (!movement->IsFalling() && currentState != EMovementState::Idle) {
			setNewState(EMovementState::Idle);
		}
	}
	else {
		EMovementState nState = bIsRunning ? EMovementState::Running : EMovementState::Walking;
		if (nState != currentState) {
			setNewState(nState);
		}
	}
}

void ACharacter_Main::Run(bool RunToggle)
{
	bIsRunning = RunToggle;
	if (RunToggle) {
		movement->MaxWalkSpeed = MaxRunSpeed;
		movement->MaxAcceleration = MaxRunAcceleration;
		if (!movement->IsFalling() && movement->Velocity.Length() > 10) {
			setNewState(EMovementState::Running);
		}
	}
	else {
		movement->MaxWalkSpeed = MaxWalkSpeed;
		movement->MaxAcceleration = MaxWalkAcceleration;
		if (!movement->IsFalling() && movement->Velocity.Length() > 10) {
			setNewState(EMovementState::Walking);
		}
	}
}
