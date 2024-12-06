// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Rail.h"

#include "VectorUtil.h"
#include "Character/Character_Main.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/Attractor/ParticleModuleAttractorBase.h"
#include "Gameplay/AttractOrb.h"
#include "Runtime/Renderer/Private/DistanceFieldAmbientOcclusion.h"

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

void ARail::OnBeginPlayerOverlap()
{
	FActorSpawnParameters SpawnParams;
	Distance = SplineComponent->GetDistanceAlongSplineAtLocation(Character->GetActorLocation(), ESplineCoordinateSpace::World);
	Orb = GetWorld()->SpawnActor<AAttractOrb>(OrbType, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	Orb->GetComponentByClass<UStaticMeshComponent>()->SetVisibility(false);

	FVector SplineVector = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Direction = FVector::DotProduct(SplineVector, Character->ActorForward);
	Direction /= FMath::Abs(Direction);

	LastForward = Character->ActorForward;
}

void ARail::OnEndPlayerOverlap()
{
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

	if (OverlappingActor != nullptr)
	{
		if (!bHit)
		{
			bHit = true;
			Character = Cast<ACharacter_Main>(OverlappingActor);
			if (Character)
			{
				OnBeginPlayerOverlap();
			}
		}
		
		if (Orb && Character)
		{
			Distance += DeltaTime * RailSpeed * Direction;
			Distance = FMath::Clamp(Distance, 0.0, SplineComponent->GetSplineLength());
			Orb->SetActorLocation(SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World));
			FVector SplineVector = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

			if ((Character->ActorForward - LastForward).Length() > std::numeric_limits<double>::epsilon())
			{
				Direction = FVector::DotProduct(SplineVector, Character->ActorForward);
				Direction /= FMath::Abs(Direction);
				LastForward = Character->ActorForward;
			}

			Orb->Inertia = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World) * LaunchStrength;
			Orb->Inertia.X *= Direction;
			Orb->Inertia.Y *= Direction;
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

