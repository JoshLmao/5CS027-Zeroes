// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Heroes/HeroBase.h"
#include "Daoko.generated.h"


/**
 * Daoko hero class - Hero specific implementations for the Daoko character
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

	/// TimerHandle to call OnBlinkDelayComplete after short duration
	FTimerHandle TimerHandle_BlinkDelay;
	/// TimerHandle to loop calling spikes until spikeCount has reached MAX_SPIKE_COUNT
	FTimerHandle TimerHandle_SpawnSpikes;
	/// TimerHandle to disable preventing movement while ability animation plays
	FTimerHandle TimerHandle_PreventMovement;
	/// TimerHandle to loop spawning missiles until missileCount has reached MAX_MISSILE_COUNT
	FTimerHandle TimerHandle_SpawnMissiles;

	/// Current amount of spikes spawned by ability one
	int m_spikeCount;
	/// Maximum amount of spikes to spawn inbetween the start and end vectors
	const int MAX_SPIKE_COUNT = 4;
	/// Start vector to spawn spikes by ability one
	FVector m_spikeStartVector;
	/// End vector point to spawn spikes by ability one
	FVector m_spikeEndVector;

	/// Current amount of spawned missiles from Daoko's ultimate
	int m_missileCount;
	/// Max amount of missiles to spawn when using Daoko's ultimate
	const int MAX_MISSILE_COUNT = 3;

	/// Timer reciever to move Daoko after short animation delay
	void OnBlinkDelayComplete();
	/// Spawns a spike inbetween the start and end vectors
	void SpawnSpike();
	/// Timer reciever to call once a duration has completed to disable preventing movement
	void OnPreventMovementFinished();
	/// Spawns a missile for Daoko's Ultimate. Will spawn until hit the MAX_MISSILE_COUNT
	void OnSpawnMissiles();
};
