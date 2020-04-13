// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZeroesGameMode.generated.h"

UCLASS(minimalapi)
class AZeroesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZeroesGameMode();

	virtual void StartPlay() override;

private:
	/// Blueprint class to use to display the game over UI
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	/// Instance of Game Over Widget displayed once game is over
	class UUserWidget* GameOverWidget;

	/// Blueprint class to use to display win UI
	TSubclassOf<class UUserWidget> WinWidgetClass;
	/// Instance of Win widget displayed once game is won
	class UUserWidget* WinWidget;

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void OnBossDeath();

	/// Finds main leader/boss Aura pawn in level
	class AAura* FindAura();
};



