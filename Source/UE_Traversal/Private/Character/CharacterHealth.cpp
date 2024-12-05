#include "Character/CharacterHealth.h"
#include "Components/CapsuleComponent.h"
#include "Character/Character_Main.h"

UCharacterHealth::UCharacterHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterHealth::BeginPlay()
{
	Super::BeginPlay();
	startHealth = health;
	canTakeHit = true;

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

void UCharacterHealth::ResetHealth()
{
	health = startHealth;
	GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);
	canTakeHit = true;
}

void UCharacterHealth::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!OtherActor->ActorHasTag("Enemy")) return;
	if(!canTakeHit) return;
	FVector ExpulseDir = (GetOwner()->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
	ExpulseDir.Z = 1;
	ExpulseDir *= HitExpulsePower;
	Cast<ACharacter_Main>(GetOwner())->Impulse(ExpulseDir);

	if(health > 0)
	{
		health--;
		OnHitByEnemy.Broadcast();
		OnHealthChanged.Broadcast(health);
		canTakeHit = false;
		if(health != 0)
		{
			GetWorld()->GetTimerManager().SetTimer(
			MyTimerHandle,
			this,
			&UCharacterHealth::OnEndCooldown,
			invulnerabilityCooldown,
			false);
		}
	}
	//UE_LOG(LogTemp,Log,TEXT("health %d"),health);
	if(health <= 0)
	{
		OnGameOver.Broadcast();
	}
}

void UCharacterHealth::AddHeart()
{
	if(health < 3)
	{
		health++;
		OnHealthChanged.Broadcast(health);
	}
	else UE_LOG(LogTemp,Log,TEXT("Already maximum health"));
}

void UCharacterHealth::TriggerGameOver()
{
	OnGameOver.Broadcast();
}

void UCharacterHealth::OnEndCooldown()
{
	canTakeHit = true;
}