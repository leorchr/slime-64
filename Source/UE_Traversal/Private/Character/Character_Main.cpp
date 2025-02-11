// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Main.h"

#include "Camera/CameraComponent.h"
#include "Character/CustomCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/AttractOrb.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/MovingPlatform.h"

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
	if (dPrdct < InclinaisonToleranceStick && dPrdct > -InclinaisonToleranceStick && movement->IsFalling() && !OtherActor->ActorHasTag(FName(TEXT("Unstickable"))))
	{
		lastWallNormal = Hit.Normal;
		lastWallHit = OtherActor;
		currentMovingWall = Cast<AMovingPlatform>(lastWallHit);
		if (currentMovingWall) {
			currSplineDistMovingWall = currentMovingWall->currentDistance;
		}
		setNewState(EMovementState::WallSticked);
		if (movement->Velocity.Length() > minimalVelocitToStick) {
			StickyTimer = TimeToUnstick;
		}
		else
		{
			StickyTimer = 0.1;
		}
		if (OtherActor->ActorHasTag(FName(TEXT("Stickable"))))
		{
			StickyTimer = std::numeric_limits<float>::max();
			FVector wNormalNoZ = lastWallNormal;
			wNormalNoZ.Z = 0;
			setNewRotationForwardTarget(wNormalNoZ);
		}
		CurrentManualUnstickTime = ManualUnstickTime;
		movement->GravityScale = 0;
		movement->Velocity = FVector::Zero();

		lastWallNormal = Hit.Normal;
		
		JumpCounter = FMath::Max(1,JumpCounter);

		movement->bNotifyApex = false;
	}
}

// Called when the game starts or when spawned
void ACharacter_Main::BeginPlay()
{
	Super::BeginPlay();
	movement->MaxWalkSpeed = MaxWalkSpeed;
	movement->MaxAcceleration = MaxWalkAcceleration;
	movement->JumpZVelocity = JumpForce;

	WallCollisionChannel = UEngineTypes::ConvertToCollisionChannel(WallCollisionTraceChannel);
	
	TArray<UStaticMeshComponent*> Comps; 
	GetComponents(Comps);
	for (int i = 0; i < Comps.Num(); i++) {
		if (Comps[i]->GetName() == "Blob") {
			blobMesh = Comps[i];
		}
		if (Comps[i]->GetName() == "Sphere") {
			LeftEyeBasePos = Comps[i]->GetRelativeLocation();
			leftEyeMesh = Comps[i];
		}
		if (Comps[i]->GetName() == "Sphere1") {
			RightEyeBasePos = Comps[i]->GetRelativeLocation();
			rightEyeMesh = Comps[i];
		}
	}
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

void ACharacter_Main::setNewRotationForwardTarget(FVector target)
{
	ActorForward = target;
	FRotator newRot = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(), target);

	newRot.Yaw -= 90;

	TargetForwardRotation = newRot;
}


void ACharacter_Main::AttachToOrb(AAttractOrb* NewOrb)
{
	Orb = NewOrb;
	JumpCounter = FMath::Max(2, JumpCounter);
	movement->GravityScale = 0.0;
	setNewState(EMovementState::Hooked);
}

void ACharacter_Main::DetachFromOrb()
{
	if (Orb != nullptr)
	{
		movement->GravityScale = BaseGravity;
		setNewState(EMovementState::Jumping);
		LaunchCharacter(Orb->Inertia, false, false);
	}
	Orb = nullptr;	
	
}

void ACharacter_Main::Impulse(FVector dir)
{
	setNewState(EMovementState::Falling);
	movement->AddImpulse(dir);
}

void ACharacter_Main::deformBasedOnVelocity()
{
	if (!SlimeDynamicMaterial) {
		SlimeDynamicMaterial = UMaterialInstanceDynamic::Create(blobMesh->GetMaterial(0), blobMesh);
	}

	float spd = movement->IsFalling() ? 0 : SpeedRatio;

	SlimeDynamicMaterial->SetScalarParameterValue("SpeedMultiplier", spd );
	FRotator meshRot = blobMesh->GetComponentRotation();
	meshRot.Yaw += 90;

	SlimeDynamicMaterial->SetVectorParameterValue("ForwardVector", meshRot.Vector());

	FVector fwd = FVector(0,1,0);

	leftEyeMesh->SetRelativeLocation(LeftEyeBasePos + (fwd * 60 * spd) );
	rightEyeMesh->SetRelativeLocation(RightEyeBasePos + (fwd * 60 * spd));

	blobMesh->SetMaterial(0, SlimeDynamicMaterial);
}

// Called every frame
void ACharacter_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Orb)
	{
		Orb->Attract(this, DeltaTime);
	}

	if (currentState == EMovementState::WallSticked && StickyTimer != 0) {
		StickyTimer = FMath::Clamp(StickyTimer - DeltaTime, 0, std::numeric_limits<float>::max());
		if (StickyTimer == 0) {
			movement->GravityScale = WallGlidingGravity;
			setNewState(EMovementState::WallSliding);
			FVector wNormalNoZ = lastWallNormal;
			wNormalNoZ.Z = 0;
			setNewRotationForwardTarget(wNormalNoZ);
		}
	}
	if (currentState == EMovementState::WallSticked || currentState == EMovementState::WallSliding) {
		//TODO : Check if the wall is still there
		FVector3d rayStart = GetActorLocation();
		FVector3d rayEnd = rayStart + (-lastWallNormal*100);

		FHitResult hitInfo;
		FHitResult hitGroundInfo;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(this);

		const bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, rayStart, rayEnd, WallCollisionChannel);

		rayEnd = rayStart + FVector::DownVector * 50;
		const bool bHitGround = GetWorld()->LineTraceSingleByChannel(hitGroundInfo, rayStart, rayEnd, WallCollisionChannel);

		if (!bHit ||bHitGround) {
			movement->GravityScale = BaseGravity;
			setNewState(EMovementState::Falling);
			movement->bNotifyApex = false;
			currentMovingWall = nullptr;
		}

		if (currentMovingWall) {
			SetActorLocation(GetActorLocation() + currentMovingWall->getNextMove(currSplineDistMovingWall, DeltaTime));
		}
		
	}
	FVector vel = movement->Velocity;
	vel.Z = 0;
	SpeedRatio = vel.Length() / MaxRunSpeed;

	FRotator newRot = UKismetMathLibrary::RInterpTo(blobMesh->GetComponentRotation(), TargetForwardRotation, DeltaTime, RotationSpeed);
	blobMesh->SetWorldRotation(newRot);
	deformBasedOnVelocity();
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
	movement->JumpZVelocity = JumpForce;
	movement->bNotifyApex = true;
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
	movement->GravityScale = BaseGravity;
	currentMovingWall = nullptr;
	
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
	JumpCounter = FMath::Min(JumpCounter, 1);
	setNewState(EMovementState::Falling);
	movement->GravityScale = ApexGravity;
}

void ACharacter_Main::Move(FVector2d Direction)
{
	lastMoveDir = Direction;
	if (currentState != EMovementState::WallSliding && currentState != EMovementState::WallSticked)
	{
		FVector3d newDir = FVector::Zero();
		if (Direction.Y != 0.f) {
			FVector3d cameraForward = Camera->GetForwardVector();
			cameraForward.Z = 0;
			cameraForward.Normalize();
			const FVector3d dir = cameraForward * Direction.Y;
			newDir += dir;
			AddMovementInput(dir);
		}
		if (Direction.X != 0.f) {
			FVector3d cameraRight = Camera->GetRightVector();
			cameraRight.Z = 0;
			cameraRight.Normalize();
			const FVector3d dir = cameraRight * Direction.X;
			newDir += dir;
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
			newDir.Z = 0;
			setNewRotationForwardTarget(newDir);
		}
	}
	else {
		FVector dirWished = FVector::Zero();
		if (Direction.Y != 0.f) {
			FVector3d cameraForward = Camera->GetForwardVector();
			cameraForward.Z = 0;
			cameraForward.Normalize();
			const FVector3d dir = cameraForward * Direction.Y;
			dirWished += dir;


		}
		if (Direction.X != 0.f) {
			FVector3d cameraRight = Camera->GetRightVector();
			cameraRight.Z = 0;
			cameraRight.Normalize();
			const FVector3d dir = cameraRight * Direction.X;
			dirWished += dir;
		}
		if (dirWished.Length() > 0.5) {
			float comparisonWallNormal = FVector::DotProduct(dirWished, lastWallNormal);
			if (comparisonWallNormal > WallUnstickTolerance) {
				if (CurrentManualUnstickTime == 0) {
					movement->GravityScale = BaseGravity;
					setNewState(EMovementState::Falling);
					movement->bNotifyApex = false;
					currentMovingWall = nullptr;
				}
				else {
					CurrentManualUnstickTime = FMath::Clamp(CurrentManualUnstickTime - GetWorld()->GetDeltaSeconds(), 0, ManualUnstickTime);
				}
			}
			else {
				CurrentManualUnstickTime = ManualUnstickTime;
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

void ACharacter_Main::CharacterJump()
{
	if (currentState != EMovementState::WallSliding && currentState != EMovementState::WallSticked) {
		if (JumpCounter > 1) {
			movement->JumpZVelocity = JumpForce;
		}
		else {
			movement->JumpZVelocity = SecondJumpForce;
			if (CanJumpInternal_Implementation()) {
				if (lastMoveDir.Length() > 0.1) {
					FVector vel = movement->Velocity;
					vel.Z = 0;
					// float spd = vel.Length();
					float spd = HorizontalWallJumpForce / 1.2f;
					FVector trueDir = lastMoveDir.Y * Camera->GetForwardVector() + lastMoveDir.X * Camera->GetRightVector();
					movement->Velocity = FVector(trueDir.X * spd, trueDir.Y * spd, movement->Velocity.Z);
 				}
			}
		}
		Jump();
	}
	else {
		FVector WallJumpForce =  FVector(lastWallNormal.X * HorizontalWallJumpForce, lastWallNormal.Y * HorizontalWallJumpForce, VerticalWallJumpForce);
		movement->AddImpulse(WallJumpForce,true);
		setNewState(EMovementState::Jumping);
		movement->bNotifyApex = true;
		movement->GravityScale = BaseGravity;
		FVector wNormalNoZ = lastWallNormal;
		wNormalNoZ.Z = 0;
		setNewRotationForwardTarget(wNormalNoZ);
	}
}

void ACharacter_Main::changeMesh(UStaticMesh* newMesh)
{
	blobMesh->SetStaticMesh(newMesh);
}
