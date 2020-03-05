// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Minion.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMinionAttackSignature, AMinion*, minion);

UCLASS()
class ZEROES_API AMinion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Visibility range when Idle
	float IdleRange;
	/// Speed to chase to the player
	float ChaseSpeed;
	/// Maximum range before the enemy stops chasing
	float ChaseRange;

	/// Duration in seconds to cooldown between attacks
	float AtkCooldownDuration;
	/// Can the minion attack
	bool bCanAttack;
	/// Amount of damage the minion deals
	float AttackDamage = 10.0f;
	/// Minimum distance from target to attack
	float AttackMinDistance;

	bool bIsAttacking;

	/* Events */
	UPROPERTY(BlueprintAssignable)
	FMinionAttackSignature OnMinionAttacking;

private:
	/// Reference to the current player pawn
	class APawn* PlayerPawn;
	/// Reference to the AI Controller to control path finding
	class AAIController* AIController;

	FVector m_spawnLocation;

	enum BehaviourStates { IDLE, CHASE, ATTACK };
	BehaviourStates State = BehaviourStates::IDLE;

	enum GameEvents { ON_START, ON_UPDATE };
	GameEvents Event = GameEvents::ON_START;

	void FSMUpdate(float DeltaTime);
	void SetState(BehaviourStates newState);

	void IdleStart();
	void IdleUpdate();

	void ChaseStart();
	void ChaseUpdate(float DeltaTime);

	void AttackStart();
	void AttackUpdate();

	FTimerHandle TimerHandle_AttackCooldown;
	void OnAtkCooldownFinished();

	void LookAtTarget(FVector target);
};
