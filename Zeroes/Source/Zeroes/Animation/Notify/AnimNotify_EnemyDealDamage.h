// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnemyDealDamage.generated.h"

/**
 * Animation Notify of when to deal damage to the target enemy
 */
UCLASS()
class ZEROES_API UAnimNotify_EnemyDealDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
