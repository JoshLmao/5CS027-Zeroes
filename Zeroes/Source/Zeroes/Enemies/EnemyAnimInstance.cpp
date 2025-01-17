// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Zeroes.h"
#include "Enemies\EnemyBase.h"
#include "TimerManager.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	bIsAttacking = false;
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

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
		AttackCount = OwningEnemy->GetAttackCount();
	}
}

void UEnemyAnimInstance::OnAttackComplete()
{
	bIsAttacking = false;
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{

}

void UEnemyAnimInstance::OnAttacking()
{
	bIsAttacking = true;
}

void UEnemyAnimInstance::OnDeath()
{
	bIsDead = true;
}
