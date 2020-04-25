// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leaders/LeaderBase.h"
#include "Aura.generated.h"

/**
 * Aura, main boss of Zeroes
 */
UCLASS()
class ZEROES_API AAura : public ALeaderBase
{
	GENERATED_BODY()
		
public:
	AAura();

	/// Amount of damage Aura's ability does to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura Properties")
	float AbilityDamage;

protected:
	virtual void AbilityStart() override;
	virtual void AbilityUpdate() override;

	virtual void PerformAbility() override;

private:
	/// TimerHandle to control when ability is complete
	FTimerHandle TimerHandle_AbilityFinish;

	/// Timer reciever to execute Aura's ability once animation is complete
	void OnAbilityFinish();
};
