// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"
#include "TimerManager.h"
#include "Zeroes.h"
#include "Heroes\HeroBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation\AnimNode_StateMachine.h"

UHeroAnimInstance::UHeroAnimInstance()
{
	bPlayingBlinkAnim = false;
}

void UHeroAnimInstance::NativeBeginPlay()
{
	struct FAnimNode_StateMachine* stateMachine = GetStateMachineInstanceFromName(TEXT("Ground Locomotion"));
	if (stateMachine)
	{
		m_animStateMachine = stateMachine;
	}
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
			OwningHero->OnCompletedAttacking.AddDynamic(this, &UHeroAnimInstance::OnAttackComplete);

			OwningHero->OnBeginAbility.AddDynamic(this, &UHeroAnimInstance::OnBeginAbility);
			OwningHero->OnCompleteAbility.AddDynamic(this, &UHeroAnimInstance::OnCompleteAbility);
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

	if (m_animStateMachine && !bPlayingBlinkAnim)
	{
		FName name = m_animStateMachine->GetCurrentStateName();
		if (name == TEXT("Ability2_Blink"))
		{
			bPlayingBlinkAnim = true;
		}
	}
}

void UHeroAnimInstance::OnAttacking()
{
	bIsAttacking = true;
}

void UHeroAnimInstance::OnAttackComplete()
{
	bIsAttacking = false;
}

void UHeroAnimInstance::OnBeginAbility(int abilityIndex)
{
	if (abilityIndex == 0)
	{
		bAbilityOneUse = true;
	}
	else if (abilityIndex == 1)
	{
		bAbilityTwoUse = true;
		UE_LOG(LogZeroes, Log, TEXT("Ability %d AbilityTwoUse"), abilityIndex+1);
	}
}

void UHeroAnimInstance::OnCompleteAbility(int abilityIndex)
{
	if (abilityIndex == 0)
	{
		bAbilityOneUse = false;
	}
	else if (abilityIndex == 1)
	{
		bAbilityTwoUse = false;
		bPlayingBlinkAnim = false;
	}
}

