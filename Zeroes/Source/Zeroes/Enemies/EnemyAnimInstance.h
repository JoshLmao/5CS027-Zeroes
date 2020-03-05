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

	APawn* OwningPawn;
	class AMinion* OwningMinion;

	virtual void NativeInitializeAnimation() override; 

	virtual void NativeUpdateAnimation(float DeltaTimeX) override; 

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

private:
	FTimerHandle TimerHandle_AttackExpired;

	UFUNCTION()
	void OnAttacking(AMinion* minion);
	void OnAttackComplete();
};
