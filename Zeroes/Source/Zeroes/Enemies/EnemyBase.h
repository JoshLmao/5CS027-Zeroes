// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EBehaviourStates : uint8
{
	IDLE,
	CHASE,
	ATTACK,
	ABILITY,
	DEAD,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathSignature);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USActorWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Enemy Properties")
	TSubclassOf<class USActorWidget> HealthbarWidget;

	EBehaviourStates State = EBehaviourStates::IDLE;

	enum GameEvents { ON_START, ON_UPDATE };
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
	float GetHealth();
	float GetMaxHealth();
	int GetAttackCount();

	/// Called by AnimNotify to deal damage to the current taregt
	void DealDamageToTarget();

	void Notify_FinishedAttackAnim();

	virtual void FSMUpdate(float DeltaTime);

	void SetState(EBehaviourStates newState);

private:
	/// Reference to the current player pawn
	class APawn* PlayerPawn;
	/// Reference to the AI Controller to control path finding
	class AAIController* AIController;
	class UEnemyAnimInstance* AnimInstance;

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

	FTimerHandle TimerHandle_AttackCooldown;
	void OnAtkCooldownFinished();

};
