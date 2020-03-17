// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBase.h"
#include "Zeroes.h"
#include "TimerManager.h"

ALeaderBase::ALeaderBase()
{
	AttacksBetweenAbility = 2;
}

void ALeaderBase::OnAttack(AActor* attackEnemy)
{
	Super::OnAttack(attackEnemy);

	int abilityThreshold = 2;
	if (AttackCount > 0 && (AttackCount % abilityThreshold) == 0)
	{
		SetState(EBehaviourStates::ABILITY);
		UE_LOG(LogZeroes, Log, TEXT("Using '%s' Leader ability on next attack!"), *this->GetName());
	}
}

void ALeaderBase::FSMUpdate(float DeltaTime)
{
	Super::FSMUpdate(DeltaTime);

	switch (State)
	{
		case EBehaviourStates::ABILITY:
			if (Event == GameEvents::ON_START)
				AbilityStart();
			else if (Event == GameEvents::ON_UPDATE)
				AbilityUpdate();
			break;
	}
}

void ALeaderBase::AbilityStart()
{
	Event = GameEvents::ON_UPDATE;
}

void ALeaderBase::AbilityUpdate()
{
	if (bCanPerformAttack)
	{
		bCanPerformAttack = false;

		if (OnLeaderBeginAbility.IsBound())
			OnLeaderBeginAbility.Broadcast();

		PerformAbility();
	}
}

void ALeaderBase::PerformAbility()
{
}
