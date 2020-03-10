// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroesMathHelper.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UZeroesMathHelper : public UObject
{
	GENERATED_BODY()
	
public:
	/// Gets a point along a straight line in between a start and end vector, from it's percentage between 0 and 1.
	/// For example: 0.50f returns half way
	static FVector GetAnyPointAlongLine(FVector start, FVector end, float pointPercentage);
};
