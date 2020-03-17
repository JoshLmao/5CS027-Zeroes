// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "LeaderBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeaderBeginAbilitySignature);

/**
 * 
 */
UCLASS()
class ZEROES_API ALeaderBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	ALeaderBase();

	/* Events */
	/// Triggered when enemy starts their attack
	UPROPERTY(BlueprintAssignable)
	FLeaderBeginAbilitySignature OnLeaderBeginAbility;

	/// Amount of attacks the leader does before using their ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	int AttacksBetweenAbility;

protected:
	virtual void OnAttack(AActor* attackEnemy) override;

	virtual void FSMUpdate(float DeltaTime) override;

	virtual void AbilityStart();
	virtual void AbilityUpdate();

	// Function to be overridden to perform leader specific functionality
	virtual void PerformAbility();
};
