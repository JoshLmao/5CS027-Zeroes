// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Zeroes.h"
#include "Enemies\EnemyBase.h"
#include "TimerManager.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	bIsAttacking = false;
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_owningPawn = TryGetPawnOwner();
	if (m_owningPawn)
	{
		OwningEnemy = Cast<AEnemyBase>(m_owningPawn);
		if (OwningEnemy)
		{
			OwningEnemy->OnEnemyBeginAttack.AddDynamic(this, &UEnemyAnimInstance::OnAttacking);
			OwningEnemy->OnEnemyDeath.AddDynamic(this, &UEnemyAnimInstance::OnDeath);
		}
		else
		{
			UE_LOG(LogZeroes, Warning, TEXT("Unable to cast pawn to minion"));
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
	if (OwningEnemy)
	{
		SpeedSq = OwningEnemy->GetVelocity().SizeSquared();
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{

}

void UEnemyAnimInstance::OnAttacking()
{
	bIsAttacking = true;
	float attackAnimDuration = 1.1f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackExpired, this, &UEnemyAnimInstance::OnAttackComplete, attackAnimDuration, false);
}

void UEnemyAnimInstance::OnDeath()
{
	bIsDead = true;
}

void UEnemyAnimInstance::OnAttackComplete()
{
	bIsAttacking = false;
}