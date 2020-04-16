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

UCLASS()
class ZEROES_API AHeroBase : public AZeroesCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroBase();

	float AbilityOneCooldown;
	float AbilityTwoCooldown;
	float AbilityThreeCooldown;
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
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FHeroBeginAttackSignature OnBeginAttacking;

	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FHeroCompleteAttackSignature OnCompletedAttacking;

	UPROPERTY(BlueprintAssignable)
	FHeroCancelAttackingSignature OnCancelAttacking;

	UPROPERTY(BlueprintAssignable)
	FHeroBeginAbilitySignature OnBeginAbility;

	UPROPERTY(BlueprintAssignable)
	FHeroCompleteAbilitySignature OnCompleteAbility;

	UPROPERTY(BlueprintAssignable)
	FHeroDeathSignature OnHeroDeath;

	/// Deals damage to the current target, if there is one
	void DealDamageToTarget();
	/// Check if the player/hero has died
	bool IsDead();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	class USoundBase* AttackSound;
	class USoundBase* WalkingSound;
	class USoundBase* DeathSound;
	class USoundBase* AbilityOneSound;
	class USoundBase* AbilityTwoSound;
	class USoundBase* AbilityThreeSound;
	class USoundBase* AbilityUltimateSound;

	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void UseAbilityOne();
	virtual void UseAbilityTwo();
	virtual void UseAbilityThree();
	virtual void UseUltimate();

	virtual void CancelAttack();

	virtual void OnDeath();

	/// Handles when controller begins travelling to it's target actor
	UFUNCTION()
	void HandleTravelToActor();
	/// Handles when controller has reached it's target actor
	UFUNCTION()
	void HandleReachedActor();

	class AZeroesPlayerController* GetZeroesPlayerController();

private:
	class AZeroesPlayerController* m_playerController;
	class AHeroState* m_heroState;

	FTimerHandle TimerHandle_AbilityOneCooldown;
	FTimerHandle TimerHandle_AbilityTwoCooldown;
	FTimerHandle TimerHandle_AbilityThreeCooldown;
	FTimerHandle TimerHandle_UltimateCooldown;
	FTimerHandle TimerHandle_AttackCooldown;
	FTimerHandle TimerHandle_WalkSoundLoop;

	enum PlayerStates { IDLE, ATTACKING, WALKING };
	PlayerStates State = PlayerStates::IDLE;

	enum StateEvents { ON_START, ON_UPDATE };
	StateEvents StateEvent = StateEvents::ON_START;

	void SM_Update();
	void SetState(PlayerStates newState);

	void AttackStart();
	void AttackUpdate();

	/*Input handlers for Ability/Ultimate*/
	void UseAbilityOnePressed();
	void UseAbilityTwoPressed();
	void UseAbilityThreePressed();
	void UseUltimatePressed();
	// Input handler for CancelCommand
	void UseCancelCommandPressed();

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

	/// Plays the current set walking sound
	void PlayWalkSound();
};
