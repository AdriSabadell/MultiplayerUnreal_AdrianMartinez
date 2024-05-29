// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeliveryEatsGameMode.h"
#include "DeliveryEatsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeliveryEatsGameMode::ADeliveryEatsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
