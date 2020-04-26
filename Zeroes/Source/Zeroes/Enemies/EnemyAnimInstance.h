// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * Enemy Animation Instance - Contains useful variables for knowing states of enemy
 */
UCLASS()
class ZEROES_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

public:
	/// Current speed squared of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float SpeedSq;

	/// Is the enemy currently attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsAttacking;

	/// Is the enemy dead
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsDead;

	/// Current amount of attacks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int AttackCount;

	/// Current owning enemy of the animation instance, can be null if cast fails
	class AEnemyBase* OwningEnemy;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	/// Called from AnimNotify to stop attacking
	void OnAttackComplete();

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

private:
	/// Current owning pawn of the animation instance
	APawn* m_owningPawn;

	/// Callback function to handle listening to AEnemyBase's OnEnemyBeginAttack event
	UFUNCTION()
	void OnAttacking();

	/// Callback function to handle listening to AEnemyBase's OnEnemyDeath event
	UFUNCTION()
	void OnDeath();
};
