// Fill out your copyright notice in the Description page of Project Settings.


#include "Daoko.h"

#include "Zeroes.h"
#include "TimerManager.h"
#include "ZeroesPlayerController.h"

ADaoko::ADaoko()
{
	BlinkDistance = 500.0f;
}

void ADaoko::UseAbilityOne()
{
	Super::UseAbilityOne();
}

void ADaoko::UseAbilityTwo()
{
	Super::UseAbilityTwo();

	float duration = 0.25f;
	GetWorldTimerManager().SetTimer(TimerHandle_BlinkDelay, this, &ADaoko::OnBlinkDelayComplete, duration, false);
}

void ADaoko::UseAbilityThree()
{
	Super::UseAbilityThree();

}

void ADaoko::UseUltimate()
{
	Super::UseUltimate();

}

void ADaoko::OnBlinkDelayComplete()
{
	// Blink Strike
	FVector blinkLocation = this->GetActorLocation() + this->GetActorForwardVector() * BlinkDistance;
	this->SetActorLocation(blinkLocation);
}
