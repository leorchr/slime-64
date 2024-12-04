// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Main.h"

#include "Camera/CameraComponent.h"
#include "Character/CustomCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/AttractOrb.h"

// Sets default values
ACharacter_Main::ACharacter_Main(const FObjectInitializer& ObjectInitializer)
{

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacter_Main::OnCompHit);

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

void ACharacter_Main::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float dPrdct = FVector::DotProduct(Hit.Normal, FVector::UpVector);
	UE_LOG(LogTemp, Log, TEXT("Hit Something | Normal : %f"), dPrdct);
	if (dPrdct < 0.05 && dPrdct > -0.05) {
		movement->GravityScale = 0;
		movement->Velocity.Z = 0;
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
	if (newState != currentState) {
		OnStateChange.Broadcast(currentState, newState);
		currentState = newState;
		const FString Message = UEnum::GetValueAsString(newState);
		UE_LOG(LogTemp, Log, TEXT("New State : %s"), *Message)
	}
}


void ACharacter_Main::AttachToOrb(AAttractOrb* NewOrb)
{
	Orb = NewOrb;
	JumpCounter = FMath::Max(1, JumpCounter);
	movement->GravityScale = 0.0;
}

void ACharacter_Main::DetachFromOrb()
{
	if (Orb != nullptr)
	{
		movement->GravityScale = BaseGravity;
	}
	Orb = nullptr;	
}

// Called every frame
void ACharacter_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Orb)
	{
		Orb->Attract(this, DeltaTime);
	}
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

	movement->GravityScale = BaseGravity;

	if (lastMoveDir.Length() == 0) {
		setNewState(EMovementState::Idle);
	}
	else {
		setNewState(bIsRunning ? EMovementState::Running : EMovementState::Walking);
	}
}

void ACharacter_Main::OnJumped_Implementation()
{
	DetachFromOrb();
	
	Super::OnJumped_Implementation();
	movement->bNotifyApex = true;
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
	Super::NotifyJumpApex();
	setNewState(EMovementState::Falling);
	movement->GravityScale = ApexGravity;
}

void ACharacter_Main::Move(FVector2d Direction)
{
	lastMoveDir = Direction;
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
		if (!movement->IsFalling()) {
			EMovementState nState = bIsRunning ? EMovementState::Running : EMovementState::Walking;
			if (nState != currentState) {
				setNewState(nState);
			}
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
