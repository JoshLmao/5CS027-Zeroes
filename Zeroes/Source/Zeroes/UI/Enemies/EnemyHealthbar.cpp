// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthbar.h"
#include "Zeroes.h"
#include "Enemies\EnemyBase.h"

void UEnemyHealthbar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (m_enemy)
	{
		Health = m_enemy->GetHealth();
		MaxHealth = m_enemy->GetMaxHealth();
		HealthPercent = Health / MaxHealth;
	}
	//UE_LOG(LogZeroes, Log, TEXT("Ticking Enemy is: %s - Health: %f"), m_enemy ? TEXT("YES") : TEXT("NO"), Health);
}

void UEnemyHealthbar::SetOwningActor(AActor* NewOwner)
{
	Super::SetOwningActor(NewOwner);

	m_enemy = Cast<AEnemyBase>(NewOwner);
}