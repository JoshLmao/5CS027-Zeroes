// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EBehaviourStates : uint8
{
	/// Enemy is idle, waiting for combat
	IDLE,
	/// Chases the player to within melee/ranged distance
	CHASE,
	/// Enemy uses their standard attack
	ATTACK,
	/// Enemy use their ability
	ABILITY,
	/// Enemy is dead
	DEAD,
	/// Circle strafe around the enemy
	STRAFE,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathSignature);

/*
* Enemy base class for creating new enemies that can attack the player
*/
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

	virtual void OnAttack(AActor* attackEnemy);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when this enemy takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/// Current health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float Health;

	/// Maximum health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float MaxHealth;

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
	/// Amount of damage the enemy deals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float AttackDamage;
	/// Minimum distance from target to attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float AttackMinDistance;

	/// Amount of attack the enemy has performed
	UPROPERTY(BlueprintReadOnly)
	int AttackCount;

	/// Current widget component that represents the enemy's healthbar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USActorWidgetComponent* WidgetComponent;

	/// Class to use to display above the enemy as their healthbar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Enemy Properties")
	TSubclassOf<class USActorWidget> HealthbarWidget;

	/// Sound to use when enemy uses it's attack
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	class USoundBase* AttackSound;

	/// Sound to use when the enemy dies
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	class USoundBase* DeathSound;

	/// Current state of the enemy
	EBehaviourStates State = EBehaviourStates::IDLE;

	/// events of the enemy's state machine
	enum GameEvents { ON_START, ON_UPDATE };
	/// Current event of the enemy's state machine
	GameEvents Event = GameEvents::ON_START;

	/* Events */
	/// Triggered when enemy starts their attack
	UPROPERTY(BlueprintAssignable)
	FEnemyAttackSignature OnEnemyBeginAttack;

	/// Triggered when enemy has lost all health and is dead
	UPROPERTY(BlueprintAssignable)
	FEnemyDeathSignature OnEnemyDeath;

	/// Can the enemy attack
	bool bCanPerformAttack;

	/* Functions */
	/// Gets the current health of the enemy
	float GetHealth();
	/// Gets the max health of the enemy
	float GetMaxHealth();
	/// Get the current attack count of the enemy
	int GetAttackCount();

	/// Called by AnimNotify to deal damage to the current taregt
	void DealDamageToTarget();

	void Notify_FinishedAttackAnim();

	/// Update's the finite state machine of the enemy
	virtual void FSMUpdate(float DeltaTime);

	/// Set the state of the FSM to a new state
	void SetState(EBehaviourStates newState);

protected:
	/// Reference to the current player pawn
	class APawn* PlayerPawn;
	/// Reference to the AI Controller to control path finding
	class AAIController* AIController;
	/// Reference to animation instance
	class UEnemyAnimInstance* AnimInstance;

	/// Once the enemy dies
	virtual void OnDeath();

private:
	/// Start spawn location of the enemy
	FVector m_spawnLocation;

	/// Has the actor been dead for too long and needs to be destroyed
	bool m_bDeathTimedOut;

	///Rate at which to sink the actor into the floor
	float m_deathSinkRate;

	void IdleStart();
	void IdleUpdate();

	void ChaseStart();
	void ChaseUpdate(float DeltaTime);

	void AttackStart();
	void AttackUpdate();

	void DeadStart();
	void DeadUpdate();

	/// TimerHandle to control the enemy's current attack
	FTimerHandle TimerHandle_AttackCooldown;
	/// Timer reciever to allow the enemy to attack once cooldown is complete
	void OnAtkCooldownFinished();

};
