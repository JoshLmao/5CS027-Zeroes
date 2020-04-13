// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZeroesGameMode.h"
#include "Zeroes.h"
#include "ZeroesPlayerController.h"
#include "ZeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/PlayerHUD.h"
#include "Heroes\HeroState.h"
#include "Kismet\GameplayStatics.h"
#include "Heroes\HeroBase.h"

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

void AZeroesGameMode::StartPlay()
{
	Super::StartPlay();
	
	const UWorld* world = GetWorld();
	AHeroBase* player = Cast<AHeroBase>(UGameplayStatics::GetPlayerPawn(Cast<UObject>(world), 0));
	if (player)
	{
		player->OnHeroDeath.AddDynamic(this, &AZeroesGameMode::OnPlayerDeath);
	}
}

void AZeroesGameMode::OnPlayerDeath()
{
	UE_LOG(LogZeroes, Log, TEXT("PLAYER HAS DIED!"));
}

void AZeroesGameMode::OnBossDeath()
{
}
