// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Controller_Main.generated.h"

/**
 * 
 */
UCLASS()
class UE_TRAVERSAL_API AController_Main : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

protected:
	class ACharacter_Main* character = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	class UInputMappingContext* inputMapping;
	//Inputs
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput|Movement")
	class UInputAction* inputActionMove;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput|Movement")
	class UInputAction* inputActionJump;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput|Movement")
	class UInputAction* inputActionRun;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput|Look")
	class UInputAction* inputActionLook;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput|Pause")
	class UInputAction* inputActionPause;

	//Movement Functions
	void MovePlayer(const struct FInputActionValue& value);
	void Jump(const struct FInputActionValue& value);
	void Run(const struct FInputActionValue& value);
	void Rotate(const struct FInputActionValue& value);

	//Pause Function
	UFUNCTION(BlueprintImplementableEvent)
	void PauseMenu(const struct FInputActionValue& value);
};
