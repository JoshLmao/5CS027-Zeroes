// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Zeroes.h"
#include "Enemies\Minion.h"
#include "TimerManager.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	bIsAttacking = false;
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
	if (OwningPawn)
	{
		OwningMinion = Cast<AMinion>(OwningPawn);
		OwningMinion->OnMinionAttacking.AddDynamic(this, &UEnemyAnimInstance::OnAttacking);
	}
	else
	{
		UE_LOG(LogZeroes, Warning, TEXT("Unable to cast pawn to minion"));
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	if (OwningMinion)
	{
		SpeedSq = OwningMinion->GetVelocity().SizeSquared();
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{

}

void UEnemyAnimInstance::OnAttacking(AMinion* minion)
{
	bIsAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackExpired, this, &UEnemyAnimInstance::OnAttackComplete, 1.1f, false);
}

void UEnemyAnimInstance::OnAttackComplete()
{
	bIsAttacking = false;
}