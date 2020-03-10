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
	FTimerHandle TimerHandle_SpawnSpikes;
	FTimerHandle TimerHandle_PreventMovement;

	int m_spikeCount;
	FVector m_spikeStartVector;
	FVector m_spikeEndVector;

	void OnBlinkDelayComplete();
	void SpawnSpike();
	void OnPreventMovementFinished();
};
