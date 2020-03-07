// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

	APlayerHUD();

public:
	UPROPERTY(BlueprintReadOnly)
	float Health;

protected:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere)
	class UUserWidget* CurrentWidget;

	class AHeroState* HeroState;
};
