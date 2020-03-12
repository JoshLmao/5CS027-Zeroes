// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBase.h"

ALeaderBase::ALeaderBase()
{

}

void ALeaderBase::OnAttack(AActor* attackEnemy)
{
	Super::OnAttack(attackEnemy);

	int abilityThreshold = 10;
	if (AttackCount % abilityThreshold == 0)
	{
		SetState(EBehaviourStates::ABILITY);
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
}

void ALeaderBase::AbilityUpdate()
{
}
