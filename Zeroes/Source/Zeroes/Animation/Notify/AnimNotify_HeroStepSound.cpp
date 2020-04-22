// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_HeroStepSound.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zeroes.h"


UAnimNotify_HeroStepSound::UAnimNotify_HeroStepSound()
{
	// Set default values for volume and pitch range
	Volume = 1.0f;
	PitchRange = FVector2D(0.9f, 1.1f);
}

void UAnimNotify_HeroStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* actor = MeshComp->GetOwner();
	UWorld* const world = actor->GetWorld();
	if (actor && WalkSound && world)
	{
		UGameplayStatics::PlaySoundAtLocation(world, WalkSound, actor->GetActorLocation(), Volume, FMath::RandRange(PitchRange.X, PitchRange.Y));
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to play walk sound. Actor, Sound or World is missing!"));
	}
}
