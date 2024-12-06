// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/MovingPlatform.h"
#include "Components/SplineComponent.h"

AMovingPlatform::AMovingPlatform(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, "Spline");
	SetRootComponent(spline);

	mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	if (spline)
	{
		mesh->SetupAttachment(spline);
	}
}


// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AMovingPlatform::getNextMove(float& currDist, float deltaTime)
{
	FVector oldPos = spline->GetLocationAtDistanceAlongSpline(currDist, ESplineCoordinateSpace::World);
	currDist = currDist + deltaTime * moveSpeed * currentDirection;
	currentDistance = FMath::Clamp(currDist, 0.0, spline->GetSplineLength());
	FVector newPos = spline->GetLocationAtDistanceAlongSpline(currDist, ESplineCoordinateSpace::World);
	return (newPos - oldPos);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentDistance =currentDistance + DeltaTime * moveSpeed * currentDirection;
	currentDistance = FMath::Clamp(currentDistance, 0.0, spline->GetSplineLength());
	
	mesh->SetWorldLocation(spline->GetLocationAtDistanceAlongSpline(currentDistance, ESplineCoordinateSpace::World));

	if ((currentDistance == 0 && currentDirection == -1) || (currentDistance == spline->GetSplineLength() && currentDirection == 1)) {
		currentDirection *= -1;
	}
}

