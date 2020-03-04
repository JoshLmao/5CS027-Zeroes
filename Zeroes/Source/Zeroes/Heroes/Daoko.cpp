// Fill out your copyright notice in the Description page of Project Settings.


#include "Daoko.h"

#include "Zeroes.h"

ADaoko::ADaoko()
{
}

void ADaoko::UseAbilityOne()
{
	Super::UseAbilityOne();
}

void ADaoko::UseAbilityTwo()
{
	Super::UseAbilityTwo();

	// Blink Strike
	FVector blinkLocation = this->GetActorLocation() + this->GetActorForwardVector() * 250.0f;
	this->SetActorLocation(blinkLocation);
}

void ADaoko::UseAbilityThree()
{
	Super::UseAbilityThree();

}

void ADaoko::UseUltimate()
{
	Super::UseUltimate();

}