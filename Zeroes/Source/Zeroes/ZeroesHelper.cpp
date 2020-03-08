// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroesHelper.h"
#include "Kismet/KismetMathLibrary.h"

FRotator UZeroesHelper::LookAtTarget(FVector currentVector, FVector targetVector, FRotator currentRotation)
{
	FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(currentVector, targetVector);
	currentRotation.Yaw = lookAtRotator.Yaw;
	return currentRotation;
}
