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
		sequence->bAbilityOneUse = false;
	}
}