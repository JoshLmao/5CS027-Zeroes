// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnemyDealDamage.h"
#include "Zeroes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation\AnimSequenceBase.h"
#include "Enemies/EnemyBase.h"

void UAnimNotify_EnemyDealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	AEnemyBase* enemy = Cast<AEnemyBase>(actor);
	if (enemy)
	{
		//UE_LOG(LogZeroes, Log, TEXT("%s - Dealing Damage to Target"), *enemy->GetName());
		enemy->DealDamageToTarget();
	}
}
