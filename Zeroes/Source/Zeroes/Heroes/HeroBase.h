// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZeroesCharacter.h"
#include "HeroBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroBeginAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroCompleteAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroCancelAttackingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroBeginAbilitySignature, int, abilityIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroCompleteAbilitySignature, int, abilityIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroDeathSignature);

/** 
 * Hero base class for creating new heroes which can attack and have 4 abilities
 */
UCLASS()
class ZEROES_API AHeroBase : public AZeroesCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroBase();

	/// Total cooldown time in seconds of ability one
	float AbilityOneCooldown;
	/// Total cooldown time in seconds of ability two
	float AbilityTwoCooldown;
	/// Total cooldown time in seconds of ability three
	float AbilityThreeCooldown;
	/// Total cooldown time in seconds of ulitmate
	float UltimateCooldown;

	/// Time in seconds inbetween each attack
	float AttackCooldown;
	/// Amount of damage to inflict per attack
	double AttackDamage;
	/// Amount of units inbetween hero and enemy to be able to attack
	float AttackRange;

	/// Normal camera zoom value
	float DefaultCameraZoom;
	/// Maximum amount for camera zoom
	float MaxCameraZoom;
	/// Minimum amount for camera zoom
	float MinCameraZoom;

	/// Is the hero dead
	bool bIsDead;

	/// Amount of time in seconds to play foot steps
	float WalkLoopDelay;

	/* Events */
	/// Event for when the hero begins attacking an enemy
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FHeroBeginAttackSignature OnBeginAttacking;

	/// Event for when the hero completes
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FHeroCompleteAttackSignature OnCompletedAttacking;

	/// Event for when the hero cancels it's current attack
	UPROPERTY(BlueprintAssignable)
	FHeroCancelAttackingSignature OnCancelAttacking;

	/// Event for when the hero begins an ability, contains the ability index starting from 0
	UPROPERTY(BlueprintAssignable)
	FHeroBeginAbilitySignature OnBeginAbility;

	/// Event for when the hero finishes an ability, contains the ability index starting from 0
	UPROPERTY(BlueprintAssignable)
	FHeroCompleteAbilitySignature OnCompleteAbility;

	/// Event for when the hero dies
	UPROPERTY(BlueprintAssignable)
	FHeroDeathSignature OnHeroDeath;

	/// Deals damage to the current target, if there is one
	void DealDamageToTarget();
	/// Check if the player/hero has died
	bool IsDead();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

protected:
	/// Attack sound of the hero
	class USoundBase* AttackSound;
	/// Death sound of the hero
	class USoundBase* DeathSound;
	/// Sound to play when the hero casts ability one
	class USoundBase* AbilityOneSound;
	/// Sound to play when the hero casts ability two
	class USoundBase* AbilityTwoSound;
	/// Sound to play when the hero casts ability three
	class USoundBase* AbilityThreeSound;
	/// Sound to play when the hero casts ulitmate
	class USoundBase* AbilityUltimateSound;

	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/// 
	virtual void UseAbilityOne();
	virtual void UseAbilityTwo();
	virtual void UseAbilityThree();
	virtual void UseUltimate();

	/// Cancel attack callback function which can be overidden
	virtual void CancelAttack();

	/// Death callback function which can be overidden
	virtual void OnDeath();

	/// Handles when controller begins travelling to it's target actor
	UFUNCTION()
	void HandleTravelToActor();
	/// Handles when controller has reached it's target actor
	UFUNCTION()
	void HandleReachedActor();

	/// Gets the current player controller
	class AZeroesPlayerController* GetZeroesPlayerController();

private:
	/// Current player controller of the hero
	class AZeroesPlayerController* m_playerController;
	/// Current hero state of the hero
	class AHeroState* m_heroState;

	/// TimerHandle to control the current cooldown of ability one
	FTimerHandle TimerHandle_AbilityOneCooldown;
	/// TimerHandle to control the current cooldown of ability two
	FTimerHandle TimerHandle_AbilityTwoCooldown;
	/// TimerHandle to control the current cooldown of ability three
	FTimerHandle TimerHandle_AbilityThreeCooldown;
	/// TimerHandle to control the current cooldown of ulitmate
	FTimerHandle TimerHandle_UltimateCooldown;
	/// /// TimerHandle to control the current cooldown of the current attack
	FTimerHandle TimerHandle_AttackCooldown;

	/// States the hero can be in
	enum PlayerStates { IDLE, ATTACKING, WALKING };
	/// Current state of the player
	PlayerStates State = PlayerStates::IDLE;

	/// All state events of the player states
	enum StateEvents { ON_START, ON_UPDATE };
	/// Current state event of the player state
	StateEvents StateEvent = StateEvents::ON_START;

	/// State machine update
	void SM_Update();
	/// Sets the new state of the internal state machine
	void SetState(PlayerStates newState);

	/// Hero starts an attack
	void AttackStart();
	/// Hero tick function in an attack
	void AttackUpdate();

	/*Input handlers for Ability/Ultimate*/
	void UseAbilityOnePressed();
	void UseAbilityTwoPressed();
	void UseAbilityThreePressed();
	void UseUltimatePressed();
	// Input handler for CancelCommand
	void UseCancelCommandPressed();

	/// Generic function to handle when any ability get's used
	void UseAbility(int index);

	void OnAbilOneCooldownFinished();
	void OnAbilTwoCooldownFinished();
	void OnAbilThreeCooldownFinished();
	void OnUltimateCooldownFinished();

	void OnAttackCooldownFinished();

	/// Handles controller resets engagement between itself and an enemy
	UFUNCTION()
	void HandleResetEngagement();

	/// Handles controller beginning movement target
	UFUNCTION()
	void HandleStartMovement();

	/// Handles controller ended or reached it's movement target
	UFUNCTION()
	void HandleEndedMovement();

	/// Resets the current camera to it's default zoom
	void ResetCameraZoom();
	/// Updates the camera zoom position depending on it's desired direction (+1 to zoom in, -1 to zoom out)
	void CameraZoomChanged(float Value);

	/// Renegerates player health using the current delta time
	void RegenerateHealth(float DeltaTime);
};
