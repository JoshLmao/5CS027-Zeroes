// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"
#include "TimerManager.h"
#include "Zeroes.h"
#include "Heroes\HeroBase.h"

UHeroAnimInstance::UHeroAnimInstance()
{

}

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_owningPawn = TryGetPawnOwner();
	if (m_owningPawn)
	{
		OwningHero = Cast<AHeroBase>(m_owningPawn);
		if (OwningHero)
		{
			OwningHero->OnBeginAttacking.AddDynamic(this, &UHeroAnimInstance::OnAttacking);
		}
		else
		{
			UE_LOG(LogZeroes, Warning, TEXT("Unable to cast pawn to minion"));
		}
	}
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (OwningHero)
	{
		SpeedSq = OwningHero->GetVelocity().SizeSquared();
	}
}

void UHeroAnimInstance::OnAttacking()
{
	bIsAttacking = true;
	float animDuration = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackExpired, this, &UHeroAnimInstance::OnAttackComplete, animDuration);
}

void UHeroAnimInstance::OnAttackComplete()
{
	bIsAttacking = false;
}
