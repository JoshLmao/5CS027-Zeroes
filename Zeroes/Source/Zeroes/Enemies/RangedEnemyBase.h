// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "RangedEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API ARangedEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

	ARangedEnemyBase();

public:
	virtual void OnAttack(AActor* attackEnemy) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARangedProjectile> Projectile;
};
