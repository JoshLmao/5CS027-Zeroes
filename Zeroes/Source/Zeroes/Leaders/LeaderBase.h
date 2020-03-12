// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "LeaderBase.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API ALeaderBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	ALeaderBase();

protected:
	virtual void OnAttack(AActor* attackEnemy) override;

	virtual void FSMUpdate(float DeltaTime) override;

	virtual void AbilityStart();
	virtual void AbilityUpdate();
};
