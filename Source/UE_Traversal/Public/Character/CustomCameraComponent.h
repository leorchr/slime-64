// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CustomCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE_TRAVERSAL_API UCustomCameraComponent : public UCameraComponent
{
public:
	GENERATED_BODY()
	
	UCustomCameraComponent();

	void Rotate(const struct FInputActionValue& axis);

private:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetPredictLocation();
	double GetYaw(float DeltaTime);
	double GetPitch(float DeltaTime);
	
	class ACharacter_Main *Character = nullptr;
	class USpringArmComponent *BoomStick = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float RotationInputLag = 8.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float YawLag = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float PitchLag = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float PositionWalkingLag = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float PositionRunningLag = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float PositionJumpingLag = 1.0f;

	float PositionLag = PositionWalkingLag;
	
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float ForwardDelta = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float StartZone = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Lag")
	float EndZone = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float WalkingFOV = 90.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float RunningFOV = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|FOV")
	float FOVLag = 0.1f;

	bool SeekPlayer = false;
};
