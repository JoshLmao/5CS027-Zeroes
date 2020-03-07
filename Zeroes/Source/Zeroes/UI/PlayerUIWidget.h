// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

	/// Returns a boolean of if the player can attack
	UFUNCTION(BlueprintCallable)
	bool GetCanAttackBoolean();

	/// Returns a value between 0 and 1 of the player's health
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable)
	float GetHealthRegenRate();

public:
	class AHeroState* PlayerState;

protected:
	virtual void NativeConstruct() override;
};