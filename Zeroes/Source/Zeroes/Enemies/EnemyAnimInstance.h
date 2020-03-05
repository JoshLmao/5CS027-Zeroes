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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float SpeedSq;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsAttacking;

	class AEnemyBase* OwningEnemy;

	virtual void NativeInitializeAnimation() override; 

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

private:
	FTimerHandle TimerHandle_AttackExpired;

	APawn* m_owningPawn;

	UFUNCTION()
	void OnAttacking();

	void OnAttackComplete();
};
