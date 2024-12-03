// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomCameraController.h"
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
	TWeakObjectPtr<class UCustomCameraController> CameraController = nullptr;	
};
