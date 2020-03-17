// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyAnimInstance.h"
#include "LeaderAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API ULeaderAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
	
public:
	ULeaderAnimInstance();

	/// Is the enemy currently attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leader")
	bool bIsUsingAbility;

	virtual void NativeInitializeAnimation() override;

	/// Called from AnimNotify to stop ability
	void OnAbilityComplete();

private:
	class ALeaderBase* OwningLeader;

	UFUNCTION()
	void OnBeginAbility();
};
