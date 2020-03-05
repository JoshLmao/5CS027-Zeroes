// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UHeroAnimInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float SpeedSq;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsAttacking;

	class AHeroBase* OwningHero;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:

private:
	FTimerHandle TimerHandle_AttackExpired;

	APawn* m_owningPawn;

	UFUNCTION()
	void OnAttacking();

	void OnAttackComplete();

};
