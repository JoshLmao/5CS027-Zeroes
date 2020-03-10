// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroesMathHelper.h"

FVector UZeroesMathHelper::GetAnyPointAlongLine(FVector start, FVector end, float pointPercentage)
{
	FVector pointVector;
	pointVector.X = start.X + pointPercentage * (end.X - start.X);
	pointVector.Y = start.Y + pointPercentage * (end.Y - start.Y);
	pointVector.Z = start.Z + pointPercentage * (end.Z - start.Z);
	return pointVector;
}
