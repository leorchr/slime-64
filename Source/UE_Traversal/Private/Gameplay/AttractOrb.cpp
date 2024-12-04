// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AttractOrb.h"

#include "FrameTypes.h"
#include "Character/Character_Main.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAttractOrb::AAttractOrb(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "Effect Field");
	if (!CollisionSphere) return;
	
	SetRootComponent(CollisionSphere);
	CollisionSphere->SetSphereRadius(100.0);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapOnlyBlob"));
}

void AAttractOrb::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAttractOrb::OnPlayerOverlap);
	CollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AAttractOrb::OnPlayerEndOverlap);
}

void AAttractOrb::OnPlayerOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* Component, signed int, bool, const FHitResult& HitResult)
{
	AttractTime = 0.0f;
	auto Slime = Cast<ACharacter_Main>(Actor);
	if (!Slime) return;

	Slime->movement->Velocity = FVector::ZeroVector;
	Slime->AttachToOrb(this);
}

void AAttractOrb::OnPlayerEndOverlap(UPrimitiveComponent*, AActor* Actor, UPrimitiveComponent*, signed int)
{
	auto Slime = Cast<ACharacter_Main>(Actor);
	if (!Slime) return;

	Slime->DetachFromOrb();
}

void AAttractOrb::Attract(ACharacter_Main* Character, float DeltaTime)
{
	AttractTime += DeltaTime;
	if (!OrbAttractionCurve) return;
	double AttractLerp = FMath::Clamp(OrbAttractionCurve->GetFloatValue(DeltaTime), -1.0, 1.0);
	Character->SetActorLocation(UE::Geometry::Lerp(Character->GetActorLocation(), GetActorLocation(), AttractLerp));
}
