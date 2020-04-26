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
#include "UI\PlayerUIWidget.h"
#include "Leaders/Aura.h"
#include "EngineUtils.h"

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

	// Assign GameOver UI widget
	static ConstructorHelpers::FClassFinder<UUserWidget> GOClass(TEXT("/Game/UI/DeathUI_BP"));
	if (GOClass.Class)
		GameOverWidgetClass = GOClass.Class;

	// Assign Win UI widget
	static ConstructorHelpers::FClassFinder<UUserWidget> WinClass(TEXT("/Game/UI/WinUI_BP"));
	if (WinClass.Class)
		WinWidgetClass = WinClass.Class;
}

void AZeroesGameMode::StartPlay()
{
	Super::StartPlay();
	
	// Listen to player death event to determine loss of game
	const UWorld* world = GetWorld();
	AHeroBase* player = Cast<AHeroBase>(UGameplayStatics::GetPlayerPawn(Cast<UObject>(world), 0));
	if (player)
	{
		player->OnHeroDeath.AddDynamic(this, &AZeroesGameMode::OnPlayerDeath);
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to find player and listen to Death event. Won't be able to lose game"));
	}

	// Listen to main leader death for win of game
	ALeaderBase* auraLeader = FindLeader();
	if (auraLeader)
	{
		auraLeader->OnDeath.AddDynamic(this, &AZeroesGameMode::OnLeaderDeath);
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to find Leader enemy. Won't be able to win the game!"));
	}
}

void AZeroesGameMode::OnPlayerDeath()
{
	UE_LOG(LogZeroes, Log, TEXT("Player has died"));

	if (GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		GameOverWidget->AddToViewport();
	}
}

void AZeroesGameMode::OnLeaderDeath()
{
	UE_LOG(LogZeroes, Log, TEXT("Leader has been killed! Player has won."));

	if (WinWidgetClass)
	{
		WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass);
		WinWidget->AddToViewport();
	}

	AZeroesCharacter* player = Cast<AZeroesCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AZeroesPlayerController* controller = Cast<AZeroesPlayerController>(player->GetController());
	controller->DisableInput(controller);
}

ALeaderBase* AZeroesGameMode::FindLeader()
{
	for (TActorIterator<ALeaderBase> TargetIter(GetWorld()); TargetIter; ++TargetIter) 
	{ 
		ALeaderBase* leader = *TargetIter;
		if (leader && leader->IsA(AAura::StaticClass()))
		{
			return Cast<AAura>(leader);
		}
	}

	return nullptr;
}
