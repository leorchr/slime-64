// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class UE_TRAVERSAL_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class USplineComponent* spline;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class UStaticMeshComponent* mesh;
	
	

	float currentDirection = 1;

	UPROPERTY(EditAnywhere)
	float moveSpeed = 100;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float beginProgress = 0;

	UPROPERTY(EditAnywhere)
	bool startInReverse = false;

	

public:	

	float currentDistance = 0;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector getNextMove(float& currDist, float deltaTime);

};
