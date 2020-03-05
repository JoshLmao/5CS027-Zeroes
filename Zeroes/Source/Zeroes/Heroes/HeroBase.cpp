// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroBase.h"

#include "Zeroes.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"
#include "ZeroesPlayerController.h"
#include "Enemies\EnemyBase.h"
#include "TimerManager.h"

// Sets default values
AHeroBase::AHeroBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	m_bCanUseAbilOne = m_bCanUseAbilTwo = m_bCanUseAbilThree = m_bCanUseUltimate = true;
	AbilityOneCooldown = AbilityTwoCooldown = AbilityThreeCooldown = 5.0f;
	UltimateCooldown = 15.0f;
	Health = 1000.0f;
	AttackDamage = 10.0;
	m_bCanAttack = false;
}

void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	m_playerController = Cast<AZeroesPlayerController>(GetController());
	if (m_playerController)
	{
		m_playerController->OnReachedDestActor.AddDynamic(this, &AHeroBase::HandleReachedActor);
	}
}

// Called every frame
void AHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SM_Update();
}

void AHeroBase::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// Listen to ability keys
	InputComponent->BindAction("Ability1", IE_Pressed, this, &AHeroBase::UseAbilityOnePressed);
	InputComponent->BindAction("Ability2", IE_Pressed, this, &AHeroBase::UseAbilityTwoPressed);
	InputComponent->BindAction("Ability3", IE_Pressed, this, &AHeroBase::UseAbilityThreePressed);
	InputComponent->BindAction("Ultimate", IE_Pressed, this, &AHeroBase::UseUltimatePressed);
}

float AHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float value = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= value;

	UE_LOG(LogZeroes, Log, TEXT("Player: Damage taken: %f - Health Remaining: %f"), value, Health);
	return value;
}

void AHeroBase::UseAbilityOne()
{
}

void AHeroBase::UseAbilityTwo()
{
}

void AHeroBase::UseAbilityThree()
{
}

void AHeroBase::UseUltimate()
{
}

void AHeroBase::HandleReachedActor()
{
	UE_LOG(LogZeroes, Log, TEXT("Hero reached actor. Entering Attacking state"));
	SetState(PlayerStates::ATTACKING);
}

void AHeroBase::SM_Update()
{
	if (State == PlayerStates::ATTACKING)
	{
		if (StateEvent == StateEvents::ON_START)
			AttackStart();
		else if (StateEvent == StateEvents::ON_UPDATE)
			AttackUpdate();
	}
}

void AHeroBase::SetState(PlayerStates newState)
{
	State = newState;
	StateEvent = StateEvents::ON_START;
}

void AHeroBase::AttackStart()
{
	m_bCanAttack = true;
	StateEvent = StateEvents::ON_UPDATE;
}

void AHeroBase::AttackUpdate()
{
	if (m_bCanAttack)
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(DestinationActor);
		
		if (enemy)
		{
			m_bCanAttack = false;
			float cooldown = 2.0f;
			GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &AHeroBase::OnAttackCooldownFinished, cooldown, false);
			UE_LOG(LogZeroes, Log, TEXT("Attacked '%s'. Cooling down for '%f' seconds"), *enemy->GetName(), cooldown);

			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			enemy->TakeDamage(AttackDamage, DamageEvent, nullptr, this);

			// Broadcast IsAttacking event
			if (OnBeginAttacking.IsBound())
				OnBeginAttacking.Broadcast();
		}
	}
}

void AHeroBase::UseAbilityOnePressed()
{
	if (m_bCanUseAbilOne)
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability One!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityOneCooldown, this, &AHeroBase::OnAbilOneCooldownFinished, AbilityOneCooldown, false);
		m_bCanUseAbilOne = false;

		UseAbilityOne();
	}
}

void AHeroBase::UseAbilityTwoPressed()
{
	if (m_bCanUseAbilTwo)
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability Two!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityTwoCooldown, this, &AHeroBase::OnAbilTwoCooldownFinished, AbilityTwoCooldown, false);
		m_bCanUseAbilTwo = false;

		UseAbilityTwo();
	}
}

void AHeroBase::UseAbilityThreePressed()
{
	if (m_bCanUseAbilThree)
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability Three!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityThreeCooldown, this, &AHeroBase::OnAbilThreeCooldownFinished, AbilityThreeCooldown, false);
		m_bCanUseAbilThree = false;

		UseAbilityThree();
	}
}

void AHeroBase::UseUltimatePressed()
{
	if (m_bCanUseUltimate)
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ultimate"));
		GetWorldTimerManager().SetTimer(TimerHandle_UltimateCooldown, this, &AHeroBase::OnUltimateCooldownFinished, UltimateCooldown, false);
		m_bCanUseUltimate = false;

		UseUltimate();
	}
}

void AHeroBase::OnAbilOneCooldownFinished()
{
	m_bCanUseAbilOne = true;
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability ONE"));
}

void AHeroBase::OnAbilTwoCooldownFinished()
{
	m_bCanUseAbilTwo = true;
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability TWO"));
}

void AHeroBase::OnAbilThreeCooldownFinished()
{
	m_bCanUseAbilThree = true;
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability THREE"));
}

void AHeroBase::OnUltimateCooldownFinished()
{
	m_bCanUseUltimate = true;
	UE_LOG(LogZeroes, Log, TEXT("Ultimate cooldown expired. Can use ultimate"));
}

void AHeroBase::OnAttackCooldownFinished()
{
	m_bCanAttack = true;
}
