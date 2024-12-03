#pragma once

#include"CoreMinimal.h"
#include"GameFramework/Character.h"
#include"Boss.generated.h"

UCLASS() class UE_TRAVERSAL_API ABoss : public ACharacter
{
    GENERATED_BODY()

    //Properties

    

    //Overriden private methods



    //Overriden protected methods

    protected : virtual void BeginPlay() override;

    //Overriden public methods
    
    public :	virtual void Tick(float DeltaTime) override;
    public :	virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

    //Methods

    public : ABoss();
};