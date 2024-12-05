// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Rail.h"

#include "VectorUtil.h"
#include "Character/Character_Main.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/Attractor/ParticleModuleAttractorBase.h"
#include "Gameplay/AttractOrb.h"

// Called when the game starts or when spawned
void ARail::BeginPlay()
{
	Super::BeginPlay();

	for (auto Component : K2_GetComponentsByClass(USplineMeshComponent::StaticClass()))
	{
		if (USplineMeshComponent *MeshComponent = Cast<USplineMeshComponent>(Component))
		{
			MeshComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
			MeshComponent->bAffectDistanceFieldLighting = true;
		}
	}
}

AActor* ARail::FindOverlappingActor()
{
	TArray<AActor*> OverlappingActors;
	for (auto Component : K2_GetComponentsByClass(USplineMeshComponent::StaticClass()))
	{
		if (USplineMeshComponent *MeshComponent = Cast<USplineMeshComponent>(Component))
		{
			MeshComponent->GetOverlappingActors(OverlappingActors, CharacterType);
			if (OverlappingActors.Num() > 0)
			{
				return OverlappingActors[0];
			}
		}
	}

	return nullptr;
}

ARail::ARail(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SplineComponent = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, "Spline");
	SetRootComponent(SplineComponent);
}

void ARail::OnBeginPlayerOverlap(ACharacter_Main* Character)
{
	UE_LOG(LogTemp, Log, TEXT("BeginOverlap"));
	FActorSpawnParameters SpawnParams;
	Distance = SplineComponent->GetDistanceAlongSplineAtLocation(Character->GetActorLocation(), ESplineCoordinateSpace::World);
	Orb = GetWorld()->SpawnActor<AAttractOrb>(OrbType, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	Orb->GetComponentByClass<UStaticMeshComponent>()->SetVisibility(false);
	

	FVector SplineVector = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Direction = FVector::DotProduct(SplineVector, Character->GetVelocity());
	Direction /= FMath::Abs(Direction);
}

void ARail::OnEndPlayerOverlap()
{
	UE_LOG(LogTemp, Log, TEXT("EndOverlap"));
	if (Orb)
	{
		Orb->Destroy();
		Orb = nullptr;
	}
}

void ARail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* OverlappingActor = FindOverlappingActor();
	ACharacter_Main *Character = Cast<ACharacter_Main>(OverlappingActor);

	if (Character != nullptr)
	{
		if (!bHit)
		{
			bHit = true;
			OnBeginPlayerOverlap(Character);
		}
		
		if (Orb)
		{
			Distance += DeltaTime * RailSpeed * Direction;
			Orb->SetActorLocation(SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World));
			FVector SplineVector = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			Direction = FVector::DotProduct(SplineVector, Character->ActorForward);
			Direction /= FMath::Abs(Direction);
		}
	} else
	{
		if (bHit)
		{
			bHit = false;
			OnEndPlayerOverlap();
		}
	}
}

