#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_TRAVERSAL_API UCharacterHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterHealth();
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnGameOver OnGameOver;

	UFUNCTION(BlueprintCallable)
	void ResetHealth();

private:
	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly)
	int health = 3;
	int startHealth;
};
