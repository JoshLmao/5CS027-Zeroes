// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AbilityFinished.generated.h"

/**
 * Animation Notify of once an ability is finished, using it's AbilityIndex (starting from 0)
 */
UCLASS()
class ZEROES_API UAnimNotify_AbilityFinished : public UAnimNotify
{
	GENERATED_BODY()

	/// Set the index of the ability that has just finished to notify the AnimInstance. 0 equals ability 1
	UPROPERTY(EditAnywhere)
	int AbilityIndex;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
