// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemyBase.h"
#include "Zeroes.h"
#include "Engine/World.h"
#include "Enemies/Projectile/RangedProjectile.h"

ARangedEnemyBase::ARangedEnemyBase()
{

}

void ARangedEnemyBase::OnAttack(AActor* attackEnemy)
{
	float gunOffset = 50.0f;
	FVector spawnLocation = this->GetActorLocation();
	FRotator spawnRotation = this->GetControlRotation();
	FVector forwardVector = this->GetActorForwardVector();
	forwardVector *= gunOffset;

	spawnLocation += forwardVector;
	UWorld* world = GetWorld();
	if (world)
	{
		ARangedProjectile* projectile = world->SpawnActor<ARangedProjectile>(Projectile, spawnLocation, spawnRotation);
		projectile->SetDamage(AttackDamage);

		//UE_LOG(LogZeroes, Log, TEXT("Spawned projectile at %s"), *spawnLocation.ToString());
	}
}
