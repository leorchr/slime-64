#include"Character/Components/Actor/Modifier.h"

void UModifier::BeginPlay()
{
    Super::BeginPlay();	
}

void UModifier::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UModifier::UModifier()
{
    PrimaryComponentTick.bCanEverTick = true;
}