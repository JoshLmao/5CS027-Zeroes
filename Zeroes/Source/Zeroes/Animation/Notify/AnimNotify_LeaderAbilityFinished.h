// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_LeaderAbilityFinished.generated.h"

/**
 * AnimNotift of when a Leader has finished an ability animation
 */
UCLASS()
class ZEROES_API UAnimNotify_LeaderAbilityFinished : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
