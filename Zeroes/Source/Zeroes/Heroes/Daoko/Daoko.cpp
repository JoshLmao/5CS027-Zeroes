// Fill out your copyright notice in the Description page of Project Settings.


#include "Daoko.h"

#include "Zeroes.h"
#include "TimerManager.h"
#include "ZeroesPlayerController.h"
#include "ZeroesMathHelper.h"
#include "Engine/World.h"
#include "Heroes/Daoko/Spike.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ADaoko::ADaoko()
{
	BlinkDistance = 500.0f;
	m_spikeCount = 0;

	// Load sounds ready to be played
	// Walking Sound
	static ConstructorHelpers::FObjectFinder<USoundBase> WalkSound(TEXT("/Game/Audio/Footstep02"));
	if (WalkSound.Succeeded())
		WalkingSound = WalkSound.Object;
	// Attack sound
	static ConstructorHelpers::FObjectFinder<USoundBase> attackSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Attack_01"));
	if (attackSound.Succeeded())
		AttackSound = attackSound.Object;
	// Death Sound
	static ConstructorHelpers::FObjectFinder<USoundBase> deathSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Death_01"));
	if (deathSound.Succeeded())
		DeathSound = deathSound.Object;

	/// Load all ability sounds
	static ConstructorHelpers::FObjectFinder<USoundBase> abilOneSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Ability_E_01"));
	if (abilOneSound.Succeeded())
		AbilityOneSound = abilOneSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> abilTwoSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Block_02"));
	if (abilTwoSound.Succeeded())
		AbilityTwoSound = abilTwoSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> abilThreeSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Jump_01"));
	if (abilThreeSound.Succeeded())
		AbilityThreeSound = abilThreeSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> abilUltiSound(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Ability_Ultimate_01"));
	if (abilUltiSound.Succeeded())
		AbilityUltimateSound = abilUltiSound.Object;
}

void ADaoko::UseAbilityOne()
{
	Super::UseAbilityOne();

	/* Spike Ability
	* Creates 3/4 spikes in front of the player that deals heavy damage to any enemies it hits
	*/

	SetPreventMovement(true);
	float animDuration = 1.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_PreventMovement, this, &ADaoko::OnPreventMovementFinished, animDuration, false);

	float spikeDistance = 350.0f;
	float spikeOffsetFromPlayer = 150.0f;
	m_spikeStartVector = this->GetActorLocation() + this->GetActorForwardVector() * spikeOffsetFromPlayer;
	m_spikeEndVector = this->GetActorLocation() + this->GetActorForwardVector() * spikeDistance;

	int spikeCount = 4;
	float spikeSpawnInteval = 0.15f;
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnSpikes, this, &ADaoko::SpawnSpike, spikeSpawnInteval, true);
}

void ADaoko::UseAbilityTwo()
{
	Super::UseAbilityTwo();

	float duration = 0.25f;
	GetWorldTimerManager().SetTimer(TimerHandle_BlinkDelay, this, &ADaoko::OnBlinkDelayComplete, duration, false);
}

void ADaoko::UseAbilityThree()
{
	Super::UseAbilityThree();

}

void ADaoko::UseUltimate()
{
	Super::UseUltimate();

	/* Kiss Love Heart Ability
	* Any enemies in a circle around the player get dealth damage
	*/

	SetPreventMovement(true);
	float duration = 2.8f;
	GetWorldTimerManager().SetTimer(TimerHandle_PreventMovement, this, &ADaoko::OnPreventMovementFinished, duration, false);
}

void ADaoko::OnBlinkDelayComplete()
{
	// Blink Strike
	FVector blinkLocation = this->GetActorLocation() + this->GetActorForwardVector() * BlinkDistance;
	this->SetActorLocation(blinkLocation);
}

void ADaoko::SpawnSpike()
{
	int maxSpikes = 4;
	if (m_spikeCount >= maxSpikes)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_SpawnSpikes);
		m_spikeCount = 0;
		m_spikeStartVector = m_spikeEndVector = FVector::ZeroVector;
		return;
	}

	FVector spikePoint = UZeroesMathHelper::GetAnyPointAlongLine(m_spikeStartVector, m_spikeEndVector, m_spikeCount + 1 / maxSpikes);
	// Sink into floor for rise animation
	spikePoint.Z -= 100.0f;

	ASpike* spike = GetWorld()->SpawnActor<ASpike>(ASpike::StaticClass(), spikePoint, FRotator::ZeroRotator);
	//spike->SetDamage(100.0f);

	m_spikeCount++;

	//UE_LOG(LogZeroes, Log, TEXT("Spawning spike at %s"), *spikePoint.ToString());
}

void ADaoko::OnPreventMovementFinished()
{
	SetPreventMovement(false);
}
