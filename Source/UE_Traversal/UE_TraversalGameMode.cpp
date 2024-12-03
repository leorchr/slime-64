// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_TraversalGameMode.h"
#include "UE_TraversalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_TraversalGameMode::AUE_TraversalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
