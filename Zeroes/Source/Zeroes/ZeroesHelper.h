// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroesHelper.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UZeroesHelper : public UObject
{
	GENERATED_BODY()
	
public:
	static FRotator LookAtTarget(FVector currentVector, FVector targetVector, FRotator currentRotation);

};
