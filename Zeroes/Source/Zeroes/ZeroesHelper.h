// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroesHelper.generated.h"

/**
 * Helper file with static functions
 */
UCLASS()
class ZEROES_API UZeroesHelper : public UObject
{
	GENERATED_BODY()
	
public:
	/// Returns a Rotator to set the actor's rotation to look at the targetVector. Requires the actor's currentLocation and currentRotation
	static FRotator LookAtTarget(FVector currentVector, FVector targetVector, FRotator currentRotation);

};
