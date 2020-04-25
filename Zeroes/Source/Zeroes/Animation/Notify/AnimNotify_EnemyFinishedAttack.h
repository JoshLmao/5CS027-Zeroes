// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnemyFinishedAttack.generated.h"

/**
 * Animation Notify of when an enemy's attack has completed
 */
UCLASS()
class ZEROES_API UAnimNotify_EnemyFinishedAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
