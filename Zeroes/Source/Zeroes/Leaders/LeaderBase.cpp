// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBase.h"
#include "Zeroes.h"
#include "TimerManager.h"
#include "AIController.h"
#include "ZeroesHelper.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ALeaderBase::ALeaderBase()
{
	AttacksBetweenAbility = 2;
	StrafeDuration = 10.0f;
	StrafeDistance = 4.0f;
	StrafeHealthPercent = 25.0f;

	m_strafeTheta = 0;
	m_strafeRadiusUnits = 350.0f;
	m_abilityCount = 0;
	m_bHasStrafed = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> abilitySound(TEXT("/Game/Audio/Frost_Blast"));
	if (abilitySound.Succeeded())
		AbilitySound = abilitySound.Object;
}

float ALeaderBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float dmg = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!m_bHasStrafed)
	{
		// If health is lower than 25%, move to strafe
		float percentThreshold = (MaxHealth / 100) * StrafeHealthPercent;
		UE_LOG(LogZeroes, Log, TEXT("Health: %f Threshold: %f"), Health, percentThreshold);

		if (Health <= percentThreshold)
		{
			SetState(EBehaviourStates::STRAFE);
			UE_LOG(LogZeroes, Log, TEXT("Settings State to STRAFE!"));
		}
	}

	if (Health <= 0)
	{
		if (OnDeath.IsBound())
			OnDeath.Broadcast();
	}

	return dmg;
}

void ALeaderBase::OnAttack(AActor* attackEnemy)
{
	Super::OnAttack(attackEnemy);

	int abilityThreshold = 2;
	if (AttackCount > 0 && (AttackCount % abilityThreshold) == 0)
	{
		SetState(EBehaviourStates::ABILITY);
		UE_LOG(LogZeroes, Log, TEXT("Using '%s' Leader ability on next attack!"), *this->GetName());
	}
}

void ALeaderBase::FSMUpdate(float DeltaTime)
{
	Super::FSMUpdate(DeltaTime);

	switch (State)
	{
		case EBehaviourStates::ABILITY:
			if (Event == GameEvents::ON_START)
				AbilityStart();
			else if (Event == GameEvents::ON_UPDATE)
				AbilityUpdate();
			break;
		case EBehaviourStates::STRAFE:
			if (Event == GameEvents::ON_START) 
				StrafeStart();
			else if (Event == GameEvents::ON_UPDATE)
				StrafeUpdate(DeltaTime);
	}
}

void ALeaderBase::AbilityStart()
{
	Event = GameEvents::ON_UPDATE;
	m_abilityCount++;
}

void ALeaderBase::AbilityUpdate()
{
	if (bCanPerformAttack)
	{
		bCanPerformAttack = false;

		if (OnLeaderBeginAbility.IsBound())
			OnLeaderBeginAbility.Broadcast();

		PerformAbility();
	}
}

void ALeaderBase::PerformAbility()
{
	if (AbilitySound)
		UGameplayStatics::PlaySoundAtLocation(this, AbilitySound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
}

void ALeaderBase::StrafeStart()
{
	Event = GameEvents::ON_UPDATE;
	m_bHasStrafed = true;

	m_strafeRadiusUnits = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation()) * StrafeDistance;
	m_strafeTheta = FMath::Acos((this->GetActorLocation().X - PlayerPawn->GetActorLocation().X) / m_strafeRadiusUnits);
	
	if ((this->GetActorLocation().X - PlayerPawn->GetActorLocation().X) < 0 && (this->GetActorLocation().Y - PlayerPawn->GetActorLocation().Y) < 0)
	{
		m_strafeTheta = -m_strafeTheta;
	}
	if ((this->GetActorLocation().X - PlayerPawn->GetActorLocation().X) > 0 && (this->GetActorLocation().Y - PlayerPawn->GetActorLocation().Y) < 0)
	{
		m_strafeTheta = -m_strafeTheta;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_StrafeDelay, this, &ALeaderBase::OnStrafeFinish, StrafeDuration);
}

void ALeaderBase::StrafeUpdate(float DeltaTime)
{
	// Get the actors cirle strafe location.
	m_strafePosition.X = PlayerPawn->GetActorLocation().X + m_strafeRadiusUnits * FMath::Cos(m_strafeTheta);
	m_strafePosition.Y = PlayerPawn->GetActorLocation().Y + m_strafeRadiusUnits * FMath::Sin(m_strafeTheta);
	m_strafePosition.Z = this->GetActorLocation().Z;
	
	// Update theta and set location
	float strafeMoveRate = 0.5f;
	m_strafeTheta += strafeMoveRate * DeltaTime;
	
	AIController->MoveToLocation(m_strafePosition);

	/// Look at player if setting is enabled
	if (bStrafeLookAtPlayer)
		this->SetActorRotation(UZeroesHelper::LookAtTarget(this->GetActorLocation(), PlayerPawn->GetActorLocation(), this->GetActorRotation()));
}

void ALeaderBase::OnStrafeFinish()
{
	// Change to Chase incase Ledaer moved out of range
	UE_LOG(LogZeroes, Log, TEXT("Strafe Delay finished, changing state to Chase"));
	SetState(EBehaviourStates::CHASE);
}
