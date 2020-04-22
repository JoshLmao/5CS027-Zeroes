// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_HeroStepSound.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UAnimNotify_HeroStepSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_HeroStepSound();

	/// Set sound to play when animation steps
	UPROPERTY(EditAnywhere)
	class USoundBase* WalkSound;

	/// Volume to play back sound at
	UPROPERTY(EditAnywhere)
	float Volume;

	/// Min (X) and Max (Y) values to randomly select the pitch between
	UPROPERTY(EditAnywhere)
	FVector2D PitchRange;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
