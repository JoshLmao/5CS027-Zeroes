// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroState.h"

AHeroState::AHeroState()
{
	Health = MaxHealth = 1000.0f;
	HealthRegenRate = 1.0f;
}

float AHeroState::GetHealth()
{
	return Health;
}

void AHeroState::SetHealth(float health)
{
	Health = health;
}

void AHeroState::RemoveHealth(float amount)
{
	float newHealth = Health - amount;
	if (newHealth < 0.0f)
		newHealth = 0.0f;

	Health = newHealth;
}

void AHeroState::AddHealth(float amount)
{
	float newHealth = Health + amount;
	if (newHealth > MaxHealth)
		newHealth = MaxHealth;

	SetHealth(newHealth);
}

float AHeroState::GetMaxHealth()
{
	return MaxHealth;
}

void AHeroState::SetMaxHealth(float maxHealth)
{
	MaxHealth = maxHealth;
}

bool AHeroState::GetCanAttack()
{
	return bCanAttack;
}

void AHeroState::SetCanAttack(bool canAttack)
{
	bCanAttack = canAttack;
}

float AHeroState::GetHealthRegenRate()
{
	return HealthRegenRate;
}

void AHeroState::SetHealthRegenRate(float healthRegenRate)
{
	HealthRegenRate = healthRegenRate;
}

bool AHeroState::GetAbilityCanUse(int index)
{
	if (AbilityStates.Contains(index))
		return AbilityStates[index];
	else
		return false;
}

void AHeroState::SetAbilityCanUse(int index, bool isInUse)
{
	if (AbilityStates.Contains(index))
		AbilityStates[index] = isInUse;
	else
		AbilityStates.Add(index, isInUse);
}
