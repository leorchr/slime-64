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

	void Update();
	void Rotate(const struct FInputActionValue& axis);

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	class ACharacter_Main *Character = nullptr;
};
