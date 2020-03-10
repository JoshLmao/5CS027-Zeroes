// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AbilityFinished.h"
#include "Zeroes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation\AnimSequenceBase.h"
#include "Heroes/HeroBase.h"
#include "Heroes/HeroAnimInstance.h"

void UAnimNotify_AbilityFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	AHeroBase* hero = Cast<AHeroBase>(actor);
	if (hero)
	{
		UHeroAnimInstance* sequence = Cast<UHeroAnimInstance>(hero->GetMesh()->GetAnimInstance());
		//UE_LOG(LogZeroes, Log, TEXT("AnimNotify Ability Finished: %d"), AbilityIndex);
		if (AbilityIndex == 0)
			sequence->bAbilityOneUse = false;
		else if (AbilityIndex == 1)
			sequence->bAbilityTwoUse = false;
		else if (AbilityIndex == 2)
			sequence->bAbilityThreeUse = false;
		else if (AbilityIndex == 3)
			sequence->bAbilityUltimateUse = false;
	}
}