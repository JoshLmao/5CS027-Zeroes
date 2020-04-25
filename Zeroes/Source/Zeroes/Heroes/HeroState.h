// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HeroState.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API AHeroState : public APlayerState
{
	GENERATED_BODY()

	AHeroState();
public:
	
	/// Gets the current health of the player
	float GetHealth();
	/// Sets the current health of the player
	void SetHealth(float health);
	/// Removes an amount of health from the player
	void RemoveHealth(float amount);
	/// Adds an amount of health to the player
	void AddHealth(float amount);

	/// Gets the max health of the player
	float GetMaxHealth();
	/// Sets a new maximum health of the player. (Doesn't set the player's health to the new max health)
	void SetMaxHealth(float maxHealth);
	
	/// Gets if the player can attack an enemy
	bool GetCanAttack();
	/// Sets if the player can attack an enemy
	void SetCanAttack(bool canAttack);

	/// Gets the current regenerate rate per second of health for the player
	float GetHealthRegenRate();
	/// Sets the current regenerate rate per second of health for the player
	void SetHealthRegenRate(float healthRegenRate);

	/// Gets if an ability can be used, by it's ability index (starting from 0)
	bool GetAbilityCanUse(int index);
	/// Sets if an ability can be used by it's ability index (starting from 0)
	void SetAbilityCanUse(int index, bool isInUse);

private:
	/// Current health of the player
	float Health;
	/// Maximum amount of health the player can have
	float MaxHealth;
	/// Amount of health the player regenerates per second
	float HealthRegenRate;
	/// Can the player attack an enemy
	bool bCanAttack;

	/// Map of all ability states (by index) and if they can be used/casted (true if can be cast)
	class TMap<int, bool> AbilityStates;
};
