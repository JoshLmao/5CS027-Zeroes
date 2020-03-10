// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_HeroDealDamage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Heroes/HeroBase.h"

void UAnimNotify_HeroDealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	AHeroBase* hero = Cast<AHeroBase>(actor);
	if (hero)
	{
		//UE_LOG(LogZeroes, Log, TEXT("%s - "), *hero->GetName());
		hero->DealDamageToTarget();
	}
	else
	{
		//UE_LOG(LogZeroes, Error, TEXT("Unable to cast actor to hero base"));
	}
}