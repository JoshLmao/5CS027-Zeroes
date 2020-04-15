// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "LeaderBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeaderBeginAbilitySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeaderDeathSignature);

/**
 * 
 */
UCLASS()
class ZEROES_API ALeaderBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	ALeaderBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/// Percentage of health remaining for the leader to transition to Strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	float StrafeHealthPercent;

	/// Time in seconds for the lader to strafe around the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	float StrafeDuration;

	/// Amount to times by to get the target distance to strafe around player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	float StrafeDistance;

	/// Should the leader look at the player whilst strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	bool bStrafeLookAtPlayer;

	/// Amount of attacks the leader does before using their ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	int AttacksBetweenAbility;

	/// Sound to play when leader casts their ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leader Properties")
	class USoundBase* AbilitySound;

	/* Events */
	/// Triggered when enemy starts their attack
	UPROPERTY(BlueprintAssignable)
	FLeaderBeginAbilitySignature OnLeaderBeginAbility;

	UPROPERTY(BlueprintAssignable)
	FLeaderDeathSignature OnDeath;

protected:
	virtual void OnAttack(AActor* attackEnemy) override;

	virtual void FSMUpdate(float DeltaTime) override;

	virtual void AbilityStart();
	virtual void AbilityUpdate();

	// Function to be overridden to perform leader specific functionality
	virtual void PerformAbility();

private:
	/// Current theta
	float m_strafeTheta;
	/// Amount of units of the radius to strafe around the player
	float m_strafeRadiusUnits;
	/// Current target vector of the strafe
	FVector m_strafePosition;
	/// Strafe Delay timer handle
	FTimerHandle TimerHandle_StrafeDelay;
	/// Leader should only strafe once, is true if the leader is or has used strafe
	bool m_bHasStrafed;
	
	/// Amount of times leader has used their ability
	int m_abilityCount;

	void StrafeStart();
	void StrafeUpdate(float DeltaTime);
	void OnStrafeFinish();
};
