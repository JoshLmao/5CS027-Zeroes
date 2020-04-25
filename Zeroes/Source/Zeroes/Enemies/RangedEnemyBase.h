// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "RangedEnemyBase.generated.h"

/**
 * A ranged enemy that can fire a projectile at the player
 */
UCLASS()
class ZEROES_API ARangedEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

	ARangedEnemyBase();

public:
	virtual void OnAttack(AActor* attackEnemy) override;

	/// Projectile class to use to fire at the enemy
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARangedProjectile> Projectile;
};
