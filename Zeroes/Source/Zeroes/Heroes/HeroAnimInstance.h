// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * Animation instance for player heroes to listen to it's events for animations
 */
UCLASS()
class ZEROES_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UHeroAnimInstance();

public:
	/// Current speed squared of the hero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float SpeedSq;

	/// Is the hero attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool bIsAttacking;

	/// Is Ability One used, and it's animation is being played. Once animation is finished, will be false
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bAbilityOneUse;

	/// Is Ability Two used, and it's animation is being played. Once animation is finished, will be false
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bAbilityTwoUse;

	/// Is Ability Three used, and it's animation is being played. Once animation is finished, will be false
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bAbilityThreeUse;

	/// Is Ultimate used, and it's animation is being played. Once animation is finished, will be false
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bAbilityUltimateUse;

	/// Owning hero of this animation instance, could be nullptr if cast failed
	class AHeroBase* OwningHero;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	/// Owning pawn of the anim instance
	APawn* m_owningPawn;

	/// Listens to AHeroBase's OnBeginAttack event to track if the hero is attacking
	UFUNCTION()
	void OnAttacking();

	/// Listens to AHeroBase's OnCompletedAttack event to track when the hero finishes an attack
	UFUNCTION()
	void OnAttackComplete();

	/// Listens to AHeroBase's OnBeginAbility event to track when an ability is started
	UFUNCTION()
	void OnBeginAbility(int abilityIndex);

	/// Listens to AHeroBase's OnBeginAbility event to track when an ability is finished
	UFUNCTION()
	void OnCompleteAbility(int abilityIndex);
};
