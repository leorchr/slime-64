// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Main.generated.h"

UCLASS()
class UE_TRAVERSAL_API ACharacter_Main : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Main(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseGravity = 1.f;

	//Data
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Walking")
	float MaxWalkSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Walking")
	float MaxWalkAcceleration = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Running")
	float MaxRunSpeed = 1200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Running")
	float MaxRunAcceleration = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallSliding")
	float WallGlidingGravity = 0.2f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Default Components
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent *SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly)
	class UCustomCameraComponent *Camera = nullptr;

	UCharacterMovementComponent* movement = nullptr;

	virtual bool CanJumpInternal_Implementation() const override;

	void Move(FVector2d Direction);
	void Run(bool RunToggle);
};
