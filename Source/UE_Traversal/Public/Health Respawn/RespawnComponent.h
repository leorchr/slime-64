// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health Respawn/Checkpoint.h"
#include "RespawnComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_TRAVERSAL_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URespawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACheckpoint* CurrentCheckpoint = nullptr;*/

	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0.2",ClampMax="30.0"),BlueprintReadOnly)
	float RespawnDelay = 2.f;
	

public:	
	/*UFUNCTION()
	void SetCurrentCheckpoint(ACheckpoint* Checkpoint){CurrentCheckpoint=CurrentCheckpoint=Checkpoint;}*/

		
};
