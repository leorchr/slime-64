#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "CharacterHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitByEnemy);

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

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHitByEnemy OnHitByEnemy;

	UFUNCTION(BlueprintCallable)
	void ResetHealth();

private:
	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void AddHeart();

protected:
	UPROPERTY(EditDefaultsOnly)
	int health = 3;

	UPROPERTY(EditDefaultsOnly)
	int HitExpulsePower = 500;

	int startHealth;

	UFUNCTION(BlueprintCallable)
	void TriggerGameOver();


protected:
	UFUNCTION(BlueprintCallable)
	void OnEndCooldown();
	FTimerHandle MyTimerHandle;
	bool canTakeHit = true;
	
	UPROPERTY(EditDefaultsOnly)
	float invulnerabilityCooldown = 5.0f;
};
