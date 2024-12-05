// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rail.generated.h"

UCLASS()
class UE_TRAVERSAL_API ARail : public AActor
{
	GENERATED_BODY()
	
public:	
	ARail(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnBeginPlayerOverlap(class ACharacter_Main* Character);
	UFUNCTION()
	void OnEndPlayerOverlap();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	AActor* FindOverlappingActor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spline")
	class USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spline|Mesh")
	class UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spline|Mesh")
	float SectionLength = 50.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spline|Mesh")
	FVector MeshScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rail")
	TSubclassOf<class AAttractOrb> OrbType;
	TSubclassOf<class ACharacter_Main> CharacterType;
	bool bHit = false;
	class AAttractOrb* Orb;

	float Distance = 0;
	float Direction = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Rail")
	float RailSpeed = 200.0f;
};
