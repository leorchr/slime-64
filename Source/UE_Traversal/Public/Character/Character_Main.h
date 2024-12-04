// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Main.generated.h"

/*
USTRUCT(BlueprintType)
struct FPlayerValue
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
}*/

UENUM()
enum class EMovementState : uint8
{
	Idle,
	Walking,
	Running,
	WallSliding,
	Jumping,
	Falling
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChange, EMovementState, OldState, EMovementState, NewState);

UCLASS()
class UE_TRAVERSAL_API ACharacter_Main : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Main(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStateChange OnStateChange;

	EMovementState currentState = EMovementState::Idle;

	bool bIsRunning;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseGravity = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float ApexGravity = 2.f;
	//Data
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Walking")
	float MaxWalkSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Walking")
	float MaxWalkAcceleration = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Running")
	float MaxRunSpeed = 1200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Running")
	float MaxRunAcceleration = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Jump")
	float JumpForce = 1200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Jump")
	int JumpNumber = 2;

	int JumpCounter = JumpNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallSliding")
	float WallGlidingGravity = 0.2f;

	void setNewState(EMovementState newState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector2d lastMoveDir = FVector2d::Zero();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Default Components
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent *SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly)
	class UCustomCameraComponent *Camera = nullptr;

	UCharacterMovementComponent* movement = nullptr;

	void Landed(const FHitResult& Hit) override;

	virtual void OnJumped_Implementation() override;

	virtual bool CanJumpInternal_Implementation() const override;

	virtual void NotifyJumpApex() override;

	void Move(FVector2d Direction);
	void Run(bool RunToggle);
};
