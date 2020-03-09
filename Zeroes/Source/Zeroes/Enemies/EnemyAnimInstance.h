// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

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

	class AEnemyBase* OwningEnemy;

	virtual void NativeInitializeAnimation() override; 

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	/// Called from AnimNotify to stop attacking
	void OnAttackComplete();

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

private:
	FTimerHandle TimerHandle_AttackExpired;

	APawn* m_owningPawn;

	UFUNCTION()
	void OnAttacking();

	UFUNCTION()
	void OnDeath();
};
