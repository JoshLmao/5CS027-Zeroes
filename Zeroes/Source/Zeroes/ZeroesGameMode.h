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
	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void OnBossDeath();
};



