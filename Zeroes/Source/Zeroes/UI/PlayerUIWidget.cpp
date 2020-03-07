// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIWidget.h"
#include  "Heroes/HeroState.h"

bool UPlayerUIWidget::GetCanAttackBoolean()
{
	return PlayerState ? PlayerState->GetCanAttack() : false;
}

float UPlayerUIWidget::GetHealthPercentage()
{
	return PlayerState ? PlayerState->GetHealth() / PlayerState->GetMaxHealth() : 0.5f;
}

float UPlayerUIWidget::GetHealthRegenRate()
{
	return PlayerState ? PlayerState->GetHealthRegenRate() : 0.0f;
}

bool UPlayerUIWidget::GetAbilityCanUse(int index)
{
	return PlayerState ? PlayerState->GetAbilityCanUse(index) : false;
}

void UPlayerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
