// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZeroesGameMode.h"
#include "ZeroesPlayerController.h"
#include "ZeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/PlayerHUD.h"
#include "Heroes\HeroState.h"

AZeroesGameMode::AZeroesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AZeroesPlayerController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerStateClass = AHeroState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Heroes/Daoko/Daoko_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
