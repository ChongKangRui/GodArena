// Copyright Epic Games, Inc. All Rights Reserved.

#include "GodArena_UE5GameMode.h"
//#include "GodArena_UE5Character.h"
#include "UObject/ConstructorHelpers.h"

AGodArena_UE5GameMode::AGodArena_UE5GameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Blueprint/Character/BP_Player"));
	/*if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not found any player pawn"));

	}*/
}
