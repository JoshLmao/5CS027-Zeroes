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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bAbilityOneUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bAbilityTwoUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bPlayingBlinkAnim;

	class AHeroBase* OwningHero;

	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:

private:
	APawn* m_owningPawn;
	struct FAnimNode_StateMachine* m_animStateMachine;

	FTimerHandle TimerHandle_BlinkAnimFinished;

	UFUNCTION()
	void OnAttacking();

	UFUNCTION()
	void OnAttackComplete();

	UFUNCTION()
	void OnBeginAbility(int abilityIndex);

	UFUNCTION()
	void OnCompleteAbility(int abilityIndex);
};
