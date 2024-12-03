#pragma once

#include"CoreMinimal.h"
#include"Components/ActorComponent.h"
#include"Modifier.generated.h"

UCLASS(ClassGroup=(Custom) , meta = (BlueprintSpawnableComponent)) class UE_TRAVERSAL_API UModifier : public UActorComponent
{
	   GENERATED_BODY()

    //Properties

    

    //Overriden private methods



    //Overriden protected methods

    protected : virtual void BeginPlay() override;

    //Overriden public methods
    
    public :	virtual void TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction) override;

    //Methods

    public :	UModifier();
};