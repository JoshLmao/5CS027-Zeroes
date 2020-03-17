// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_LeaderAbilityFinished.h"
#include "Zeroes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation\AnimSequenceBase.h"
#include "Leaders/LeaderBase.h"
#include "Leaders/LeaderAnimInstance.h"

void UAnimNotify_LeaderAbilityFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	ALeaderBase* leader = Cast<ALeaderBase>(actor);
	if (leader)
	{
		ULeaderAnimInstance* sequence = Cast<ULeaderAnimInstance>(leader->GetMesh()->GetAnimInstance());
		sequence->OnAbilityComplete();
	}
}