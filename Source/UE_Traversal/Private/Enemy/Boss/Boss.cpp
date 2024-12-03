#include"Enemy/Boss/Boss.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();	
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABoss::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	   Super::SetupPlayerInputComponent(PlayerInputComponent);
}

