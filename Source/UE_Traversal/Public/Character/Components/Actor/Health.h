#pragma once

#include"CoreMinimal.h"
#include"Components/ActorComponent.h"
#include"Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamagedDelegate , UHealth * , InHealthActorComponent , float , InDamagedHealthAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiedDelegate , UHealth * , InHealthActorComponent , float , InDamagedHealthAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealedDelegate , UHealth * , InHealthActorComponent , float , InHealedHealthAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecoveredDelegate , UHealth * , InHealthActorComponent , float , IHealedHealthAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifiedDelegate , UHealth * , InHealthActorComponent , float , InModifiedHealthAmount);

UCLASS(ClassGroup=(Custom) , Blueprintable , meta=(BlueprintSpawnableComponent)) class UE_TRAVERSAL_API UHealth : public UActorComponent
{
    GENERATED_BODY()

    //Properties

    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) bool bAssignHealthLimitToCurrent;
    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) float Limit;
    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) float Current;
    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) bool bAssignInvincibilityDurationToTimer;
    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) float InvincibilityTimeDuration;
    public : UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ExposeOnSpawn = true)) float InvincibilityTimeTimer;
    public : UPROPERTY(BlueprintAssignable , BlueprintCallable) FOnDamagedDelegate OnDamagedDelegate;
    public : UPROPERTY(BlueprintAssignable , BlueprintCallable) FOnDiedDelegate OnDiedDelegate;
    public : UPROPERTY(BlueprintAssignable , BlueprintCallable) FOnHealedDelegate OnHealedDelegate;
    public : UPROPERTY(BlueprintAssignable , BlueprintCallable) FOnRecoveredDelegate OnRecoveredDelegate;
    public : UPROPERTY(BlueprintAssignable , BlueprintCallable) FOnModifiedDelegate OnModifiedDelegate;

    //Overriden private methods



    //Overriden protected methods

    protected : virtual void BeginPlay() override;

    //Overriden public methods
    
    public :	virtual void TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction) override;

    //Methods

    public : UHealth();
    public : UFUNCTION(BlueprintCallable) virtual void AssignHealthLimitToCurrent_Implementation();
    public : UFUNCTION(BlueprintCallable , BlueprintNativeEvent) void AssignHealthLimitToCurrent();
    public : UFUNCTION(BlueprintCallable) virtual void AssignInvincibilityDurationToTimer_Implementation();
    public : UFUNCTION(BlueprintCallable , BlueprintNativeEvent) void AssignInvincibilityDurationToTimer();
    public : UFUNCTION(BlueprintCallable) virtual void Modify_Implementation
    (
        float InModifiedHealthAmount , bool bInNotifyAboutModification
        ,
        bool bInNotifyAboutDamage , bool bInNotifyAboutDeath
        ,
        bool bInNotifyAboutHeal , bool bInNotifyAboutRecovery
    );
    public : UFUNCTION(BlueprintCallable , BlueprintNativeEvent) void Modify
    (
        float InModifiedHealthAmount , bool bInNotifyAboutModification
        ,
        bool bInNotifyAboutDamage , bool bInNotifyAboutDeath
        ,
        bool bInNotifyAboutHeal , bool bInNotifyAboutRecovery
    );
};