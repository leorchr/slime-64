#include"Character/Components/Actor/Health.h"

void UHealth::BeginPlay()
{
    Super::BeginPlay();
    AssignHealthLimitToCurrent_Implementation();
    AssignInvincibilityDurationToTimer_Implementation();
}

void UHealth::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	    Super::TickComponent(DeltaTime , TickType , ThisTickFunction);
     InvincibilityTimeTimer += DeltaTime;
     if(Current <= 0.0F)
     {
         TimeBeforeBlackScreenTimer += DeltaTime;
         if(TimeBeforeBlackScreenTimer >= TimeBeforeBlackScreenDuration)
         {
            InvincibilityTimeTimer = -1'000'000'000.F;
            OnGotBlackScreenDelegate.Broadcast(this);
         }
     }
}

UHealth::UHealth()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAssignHealthLimitToCurrent = true;
    Limit = 3.0F;
    AssignHealthLimitToCurrent_Implementation();
    bAssignInvincibilityDurationToTimer = true;
    InvincibilityTimeDuration = 2.0F;
    AssignInvincibilityDurationToTimer_Implementation();
    bAssignTimeBeforeBlackScreenDurationToTimer = false;
    TimeBeforeBlackScreenDuration = 3.0F;
    AssignTimeBeforeBlackScreenDurationToTimer_Implementation();
}

void UHealth::AssignHealthLimitToCurrent_Implementation()
{
    if(bAssignHealthLimitToCurrent)
    {
        Current = Limit;
    }
}

void UHealth::AssignInvincibilityDurationToTimer_Implementation()
{
    if(bAssignInvincibilityDurationToTimer)
    {
        InvincibilityTimeTimer = InvincibilityTimeDuration;
    }
}

void UHealth::AssignTimeBeforeBlackScreenDurationToTimer_Implementation()
{
    if(bAssignTimeBeforeBlackScreenDurationToTimer)
    {
        TimeBeforeBlackScreenTimer = TimeBeforeBlackScreenDuration;
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
    if(InvincibilityTimeTimer >= InvincibilityTimeDuration)
    {
        InvincibilityTimeTimer = 0.0F;
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
}