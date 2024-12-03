#include"Character/Components/Actor/Health.h"

void UHealth::BeginPlay()
{
    Super::BeginPlay();
    AssignLimitHealthToCurrent_Implementation();
}

void UHealth::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	    Super::TickComponent(DeltaTime , TickType , ThisTickFunction);
}

UHealth::UHealth()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAssignLimitHealthToCurrent = true;
    Limit = 100.0;
    AssignLimitHealthToCurrent_Implementation();
}

void UHealth::AssignLimitHealthToCurrent_Implementation()
{
    if(bAssignLimitHealthToCurrent)
    {
        Current = Limit;
    }
}

void UHealth::Modify_Implementation
(
    float InModifiedHealthAmount , bool bInNotifyAboutModification
    ,
    bool bInNotifyAboutDamage , bool bInNotifyAboutDeath
    ,
    bool bInNotifyAboutHeal , bool bInNotifyAboutRecovery
)
{
    Current = FMath::Clamp(Current + InModifiedHealthAmount , 0.0F , Limit);
    if(bInNotifyAboutModification)
    {
        OnModifiedDelegate.Broadcast(this , InModifiedHealthAmount);
    }
    if(bInNotifyAboutDamage && InModifiedHealthAmount < 0.0F)
    {
        OnDamagedDelegate.Broadcast(this , FMath::Abs(InModifiedHealthAmount));
    }
    if(bInNotifyAboutDeath && Current <= 0.0F)
    {
        OnDiedDelegate.Broadcast(this , FMath::Abs(InModifiedHealthAmount));
    }
    if(bInNotifyAboutHeal && InModifiedHealthAmount > 0.0F)
    {
        OnHealedDelegate.Broadcast(this , FMath::Abs(InModifiedHealthAmount));
    }
    if(bInNotifyAboutRecovery && Current >= Limit)
    {
        OnRecoveredDelegate.Broadcast(this , FMath::Abs(InModifiedHealthAmount));
    }
}