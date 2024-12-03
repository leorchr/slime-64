#include "Character/CharacterHealth.h"
#include "Components/CapsuleComponent.h"

UCharacterHealth::UCharacterHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterHealth::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent = GetOwner()->FindComponentByClass<UCapsuleComponent>();
	if (CapsuleComponent)
	{
		CapsuleComponent->OnComponentHit.AddDynamic(this, &UCharacterHealth::OnHit);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No capsule component found on %s"), *GetOwner()->GetName());
	}
}

void UCharacterHealth::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(health > 0)
	{
		health--;
		OnHealthChanged.Broadcast(health);
	}
}

