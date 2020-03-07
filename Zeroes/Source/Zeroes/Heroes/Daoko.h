// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Heroes/HeroBase.h"
#include "Daoko.generated.h"


/**
 * 
 */
UCLASS()
class ZEROES_API ADaoko : public AHeroBase
{
	GENERATED_BODY()

public:
	ADaoko();

	/// Amount in units to blink forward
	float BlinkDistance;

protected:
	virtual void UseAbilityOne() override;
	virtual void UseAbilityTwo() override;
	virtual void UseAbilityThree() override;
	virtual void UseUltimate() override;

	FTimerHandle TimerHandle_BlinkDelay;

	void OnBlinkDelayComplete();
};
