// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnemyFinishedAttack.h"
#include "Zeroes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemies/EnemyBase.h"

void UAnimNotify_EnemyFinishedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	AEnemyBase* enemy = Cast<AEnemyBase>(actor);
	if (enemy)
	{
		UE_LOG(LogZeroes, Log, TEXT("%s - Finished AnimAttack"), *enemy->GetName());
		enemy->Notify_FinishedAttackAnim();
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to cast actor to enemy"));
	}
}