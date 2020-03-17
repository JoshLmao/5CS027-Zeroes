// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leaders/LeaderBase.h"
#include "Aura.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API AAura : public ALeaderBase
{
	GENERATED_BODY()
		
public:
	AAura();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura Properties")
	float AbilityDamage;

protected:
	virtual void AbilityStart() override;
	virtual void AbilityUpdate() override;

	virtual void PerformAbility() override;

private:
	FTimerHandle TimerHandle_AbilityFinish;

	void OnAbilityFinish();
};
