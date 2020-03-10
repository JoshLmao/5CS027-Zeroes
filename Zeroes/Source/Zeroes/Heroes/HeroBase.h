// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZeroesCharacter.h"
#include "HeroBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroBeginAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroCompleteAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroBeginAbilitySignature, int, abilityIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroCompleteAbilitySignature, int, abilityIndex);

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

	/// Normal camera zoom value
	float DefaultCameraZoom;
	/// Maximum amount for camera zoom
	float MaxCameraZoom;
	/// Minimum amount for camera zoom
	float MinCameraZoom;

	/* Events */
	UPROPERTY(BlueprintAssignable)
	FHeroBeginAttackSignature OnBeginAttacking;

	UPROPERTY(BlueprintAssignable)
	FHeroCompleteAttackSignature OnCompletedAttacking;

	UPROPERTY(BlueprintAssignable)
	FHeroBeginAbilitySignature OnBeginAbility;

	UPROPERTY(BlueprintAssignable)
	FHeroCompleteAbilitySignature OnCompleteAbility;

	void DealDamageToTarget();

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void UseAbilityOne();
	virtual void UseAbilityTwo();
	virtual void UseAbilityThree();
	virtual void UseUltimate();

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

	void UseAbility(int index);

	void OnAbilOneCooldownFinished();
	void OnAbilTwoCooldownFinished();
	void OnAbilThreeCooldownFinished();
	void OnUltimateCooldownFinished();

	void OnAttackCooldownFinished();

	UFUNCTION()
	void HandleResetEngagement();

	void ResetCameraZoom();
	void CameraZoomChanged(float Value);

	void RegenerateHealth(float DeltaTime);
};
