// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttractOrb.generated.h"

UCLASS()
class UE_TRAVERSAL_API AAttractOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttractOrb(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent_, int32 i, bool b, const FHitResult& HitResult);
	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent_, signed int i);

	void Attract(class ACharacter_Main* Character, float DeltaTime);

	bool bDeleteOnLeave = false;
	FVector Inertia = FVector::ZeroVector;
	
protected:
	virtual void BeginPlay();
	float AttractTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent *CollisionSphere = nullptr;
	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat *OrbAttractionCurve = nullptr;
};

