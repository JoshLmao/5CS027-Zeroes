// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMinionAttackSignature);

UCLASS()
class ZEROES_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when this enemy takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/// Current health of the enemy
	float Health;

	/// Visibility range when Idle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float IdleRange;

	/// Speed to chase to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float ChaseSpeed;
	/// Maximum range before the enemy stops chasing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float ChaseRange;

	/// Duration in seconds to cooldown between attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float AtkCooldownDuration;
	/// Amount of damage the minion deals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float AttackDamage;
	/// Minimum distance from target to attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float AttackMinDistance;

	/* Events */
	UPROPERTY(BlueprintAssignable)
	FMinionAttackSignature OnMinionAttacking;

private:
	/// Reference to the current player pawn
	class APawn* PlayerPawn;
	/// Reference to the AI Controller to control path finding
	class AAIController* AIController;

	/// Start spawn location of the enemy
	FVector m_spawnLocation;

	/// Can the minion attack
	bool m_bCanAttack;

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
