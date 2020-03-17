// Fill out your copyright notice in the Description page of Project Settings.


#include "Aura.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Zeroes.h"

AAura::AAura()
{
	AbilityDamage = 100.0f;
}

void AAura::AbilityStart()
{
	Super::AbilityStart();

}

void AAura::AbilityUpdate()
{
	Super::AbilityUpdate();

}

void AAura::PerformAbility()
{
	// 0.5s finish drawing circle
	float radius = 200.0f;
	float duration = 2.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_AbilityFinish, this, &AAura::OnAbilityFinish, duration);
	DrawDebugCylinder(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f), radius, 50, FColor::Black, false, duration);

}

void AAura::OnAbilityFinish()
{
	SetState(EBehaviourStates::ATTACK);
	UE_LOG(LogZeroes, Log, TEXT("Aura ability finshed. Returning to Attacking!"));
}