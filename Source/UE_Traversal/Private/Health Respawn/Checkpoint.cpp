// Fill out your copyright notice in the Description page of Project Settings.


#include "Health Respawn/Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Health Respawn/RespawnComponent.h"

// Sets default values
ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,"CollisionBox");
	if(CollisionBox)SetRootComponent(CollisionBox);

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	/*if(CollisionBox){
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ACheckpoint::OnOverlapBegin);
	}*/
	
}

/*void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	URespawnComponent* EnteringRespawnComponent = OtherActor->FindComponentByClass<URespawnComponent>();
	if(!EnteringRespawnComponent)return;
	EnteringRespawnComponent->SetCurrentCheckpoint(this);
}*/


