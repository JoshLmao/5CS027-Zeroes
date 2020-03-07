// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroBase.h"

#include "Zeroes.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ZeroesPlayerController.h"
#include "Enemies\EnemyBase.h"
#include "TimerManager.h"
#include "GameFramework\SpringArmComponent.h"
#include "Heroes\HeroState.h"

// Sets default values
AHeroBase::AHeroBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	m_bCanUseAbilTwo = m_bCanUseAbilThree = m_bCanUseUltimate = true;
	AbilityOneCooldown = AbilityTwoCooldown = AbilityThreeCooldown = 5.0f;
	UltimateCooldown = 15.0f;
	AttackDamage = 75.0;
	AttackCooldown = 2.0f;
	DefaultCameraZoom = 800.0f;
	MinCameraZoom = 800.0f;
	MaxCameraZoom = 1400.0f;
}

void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	ResetCameraZoom();

	m_playerController = Cast<AZeroesPlayerController>(GetController());
	if (m_playerController)
	{
		m_playerController->OnReachedDestActor.AddDynamic(this, &AHeroBase::HandleReachedActor);
		m_playerController->OnResetEngagement.AddDynamic(this, &AHeroBase::HandleResetEngagement);
	}

	AHeroState* state = Cast<AHeroState>(GetPlayerState());
	if (state)
	{
		m_heroState = state;

		// Set State default values
		m_heroState->SetCanAttack(true);
		// Add ability states so they can be used
		for (int i = 0; i < 4; i++) {
			m_heroState->SetAbilityCanUse(i, true);
		}
	}
}

// Called every frame
void AHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Regenerate Health every frame
	RegenerateHealth(DeltaTime);

	SM_Update();
}

void AHeroBase::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Listen to ability keys
	InputComponent->BindAction("Ability1", IE_Pressed, this, &AHeroBase::UseAbilityOnePressed);
	InputComponent->BindAction("Ability2", IE_Pressed, this, &AHeroBase::UseAbilityTwoPressed);
	InputComponent->BindAction("Ability3", IE_Pressed, this, &AHeroBase::UseAbilityThreePressed);
	InputComponent->BindAction("Ultimate", IE_Pressed, this, &AHeroBase::UseUltimatePressed);

	InputComponent->BindAxis("CameraZoom", this, &AHeroBase::CameraZoomChanged);
}

float AHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float value = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	m_heroState->RemoveHealth(value);
	if (m_heroState->GetHealth() <= 0)
	{
		// Player is Dead
	}
	else
	{
		UE_LOG(LogZeroes, Log, TEXT("Player: Damage taken: %f - Health Remaining: %f"), value, m_heroState->GetHealth());
	}

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
	StateEvent = StateEvents::ON_UPDATE;
}

void AHeroBase::AttackUpdate()
{
	if (m_heroState->GetCanAttack())
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(DestinationActor);

		if (enemy && enemy->Health > 0)
		{
			m_heroState->SetCanAttack(false);

			GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &AHeroBase::OnAttackCooldownFinished, AttackCooldown, false);
			UE_LOG(LogZeroes, Log, TEXT("Attacked '%s'. Cooling down for '%f' seconds"), *enemy->GetName(), AttackCooldown);

			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			enemy->TakeDamage(AttackDamage, DamageEvent, nullptr, this);
				
			if (enemy->Health <= 0)
			{
				// Enemy died on attack
				m_playerController->ResetTargetEnemy();
				// Reset state
				SetState(PlayerStates::IDLE);
			}

			// Broadcast IsAttacking event
			if (OnBeginAttacking.IsBound())
				OnBeginAttacking.Broadcast();
		}
	}
}

void AHeroBase::UseAbilityOnePressed()
{
	if (m_heroState->GetAbilityCanUse(0))
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability One!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityOneCooldown, this, &AHeroBase::OnAbilOneCooldownFinished, AbilityOneCooldown, false);
		m_heroState->SetAbilityCanUse(0, false);

		UseAbilityOne();
	}
}

void AHeroBase::UseAbilityTwoPressed()
{
	if (m_heroState->GetAbilityCanUse(1))
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability Two!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityTwoCooldown, this, &AHeroBase::OnAbilTwoCooldownFinished, AbilityTwoCooldown, false);
		m_heroState->SetAbilityCanUse(1, false);

		UseAbilityTwo();
	}
}

void AHeroBase::UseAbilityThreePressed()
{
	if (m_heroState->GetAbilityCanUse(2))
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability Three!"));
		GetWorldTimerManager().SetTimer(TimerHandle_AbilityThreeCooldown, this, &AHeroBase::OnAbilThreeCooldownFinished, AbilityThreeCooldown, false);
		m_heroState->SetAbilityCanUse(2, false);

		UseAbilityThree();
	}
}

void AHeroBase::UseUltimatePressed()
{
	if (m_heroState->GetAbilityCanUse(3))
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ultimate"));
		GetWorldTimerManager().SetTimer(TimerHandle_UltimateCooldown, this, &AHeroBase::OnUltimateCooldownFinished, UltimateCooldown, false);
		m_heroState->SetAbilityCanUse(3, false);

		UseUltimate();
	}
}

void AHeroBase::OnAbilOneCooldownFinished()
{
	m_heroState->SetAbilityCanUse(0, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability ONE"));
}

void AHeroBase::OnAbilTwoCooldownFinished()
{
	m_heroState->SetAbilityCanUse(1, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability TWO"));
}

void AHeroBase::OnAbilThreeCooldownFinished()
{
	m_heroState->SetAbilityCanUse(2, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability THREE"));
}

void AHeroBase::OnUltimateCooldownFinished()
{
	m_heroState->SetAbilityCanUse(3, true);
	UE_LOG(LogZeroes, Log, TEXT("Ultimate cooldown expired. Can use ultimate"));
}

void AHeroBase::OnAttackCooldownFinished()
{
	// Once attack cooldown is finished, enable attacking again
	m_heroState->SetCanAttack(true);
}

void AHeroBase::HandleResetEngagement()
{
	UE_LOG(LogZeroes, Log, TEXT("Resetting Player enegagement - idle state"));
	SetState(PlayerStates::IDLE);
}

void AHeroBase::ResetCameraZoom()
{
	GetCameraBoom()->TargetArmLength = DefaultCameraZoom;
	UE_LOG(LogZeroes, Log, TEXT("Reset camera length"));
}

void AHeroBase::CameraZoomChanged(float Value)
{
	float zoomRate = 20.0f;

	// Camera scroll zoom
	if (Value > 0.0f)
	{
		// Player is Dead
		float existing = GetCameraBoom()->TargetArmLength;
		float newValue = existing - (zoomRate * Value);
		if (newValue < MinCameraZoom)
			newValue = MinCameraZoom;

		GetCameraBoom()->TargetArmLength = newValue;
	}
	else if (Value < 0.0f)
	{
		float existing = GetCameraBoom()->TargetArmLength;
		float newValue = existing + (zoomRate * std::abs(Value));
		if (newValue > MaxCameraZoom)
			newValue = MaxCameraZoom;

		GetCameraBoom()->TargetArmLength = newValue;
	}
}

void AHeroBase::RegenerateHealth(float DeltaTime)
{
	m_heroState->AddHealth(m_heroState->GetHealthRegenRate() * DeltaTime);
}
