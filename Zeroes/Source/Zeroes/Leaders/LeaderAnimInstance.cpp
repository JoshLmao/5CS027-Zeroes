// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderAnimInstance.h"
#include "Leaders\LeaderBase.h"

ULeaderAnimInstance::ULeaderAnimInstance()
{

}

void ULeaderAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningEnemy)
	{
		OwningLeader = Cast<ALeaderBase>(OwningEnemy);
		if (OwningLeader)
		{
			OwningLeader->OnLeaderBeginAbility.AddDynamic(this, &ULeaderAnimInstance::OnBeginAbility);
		}
	}
}

void ULeaderAnimInstance::OnAbilityComplete()
{
	bIsUsingAbility = false;
}

void ULeaderAnimInstance::OnBeginAbility()
{
	bIsUsingAbility = true;
}