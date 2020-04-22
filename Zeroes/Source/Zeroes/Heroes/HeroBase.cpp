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
#include "Kismet\KismetMathLibrary.h"
#include "ZeroesHelper.h"
#include "ZeroesPlayerController.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHeroBase::AHeroBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	AbilityOneCooldown = AbilityTwoCooldown = AbilityThreeCooldown = 5.0f;
	UltimateCooldown = 15.0f;
	AttackDamage = 75.0;
	AttackCooldown = 1.0f;
	AttackRange = 175.0f;
	DefaultCameraZoom = 800.0f;
	MinCameraZoom = 800.0f;
	MaxCameraZoom = 1400.0f;
	WalkLoopDelay = 0.275f;
}

void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	ResetCameraZoom();

	m_playerController = Cast<AZeroesPlayerController>(GetController());
	if (m_playerController)
	{
		m_playerController->OnTravelDestActor.AddDynamic(this, &AHeroBase::HandleTravelToActor);
		m_playerController->OnReachedDestActor.AddDynamic(this, &AHeroBase::HandleReachedActor);
		m_playerController->OnResetEngagement.AddDynamic(this, &AHeroBase::HandleResetEngagement);
		m_playerController->OnStartMovement.AddDynamic(this, &AHeroBase::HandleStartMovement);
		m_playerController->OnEndedMovement.AddDynamic(this, &AHeroBase::HandleEndedMovement);
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
	if (!IsDead())
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
	InputComponent->BindAction("CancelCommand", IE_Pressed, this, &AHeroBase::UseCancelCommandPressed);

	InputComponent->BindAxis("CameraZoom", this, &AHeroBase::CameraZoomChanged);
}

float AHeroBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float value = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	m_heroState->RemoveHealth(value);
	if (m_heroState->GetHealth() <= 0)
	{
		OnDeath();
	}
	else
	{
		UE_LOG(LogZeroes, Log, TEXT("Player: Damage taken: %f - Health Remaining: %f"), value, m_heroState->GetHealth());
	}

	return value;
}

void AHeroBase::UseAbilityOne()
{
	if (AbilityOneSound)
		UGameplayStatics::PlaySoundAtLocation(this, AbilityOneSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
}

void AHeroBase::UseAbilityTwo()
{
	if (AbilityTwoSound)
		UGameplayStatics::PlaySoundAtLocation(this, AbilityTwoSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
}

void AHeroBase::UseAbilityThree()
{
	if (AbilityThreeSound)
		UGameplayStatics::PlaySoundAtLocation(this, AbilityThreeSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
}

void AHeroBase::UseUltimate()
{
	if (AbilityUltimateSound)
		UGameplayStatics::PlaySoundAtLocation(this, AbilityUltimateSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
}

void AHeroBase::CancelAttack()
{
	// If can't attack
	if (!m_heroState->GetCanAttack())
	{
		UE_LOG(LogZeroes, Log, TEXT("Cancelling current player attack"));
		GetWorldTimerManager().ClearTimer(TimerHandle_AttackCooldown);

		m_heroState->SetCanAttack(true);

		if (OnCancelAttacking.IsBound())
			OnCancelAttacking.Broadcast();

		SetState(PlayerStates::IDLE);
	}
}

void AHeroBase::OnDeath()
{
	// Set is dead and stop movement
	bIsDead = true;
	SetPreventMovement(true);

	// Move state incase attacking
	SetState(PlayerStates::IDLE);

	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), 1.0f, 1.0f);

	if (OnHeroDeath.IsBound())
		OnHeroDeath.Broadcast();

	/* Enable ragdoll */

	// Configure cpasule component for physics
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	SetActorEnableCollision(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
}

void AHeroBase::DealDamageToTarget()
{
	AEnemyBase* enemy = Cast<AEnemyBase>(DestinationActor);
	if (!enemy)
		return;

	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	enemy->TakeDamage(AttackDamage, DamageEvent, nullptr, this);
}

bool AHeroBase::IsDead()
{
	return m_heroState->GetHealth() <= 0;
}

void AHeroBase::HandleTravelToActor()
{
}

void AHeroBase::HandleReachedActor()
{
	UE_LOG(LogZeroes, Log, TEXT("Hero reached actor. Entering Attacking state"));
	SetState(PlayerStates::ATTACKING);
}

AZeroesPlayerController* AHeroBase::GetZeroesPlayerController()
{
	return m_playerController;
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
	AEnemyBase* enemy = Cast<AEnemyBase>(DestinationActor);
	float dist = FVector::Distance(this->GetActorLocation(), enemy->GetActorLocation());
	if (m_heroState->GetCanAttack() && dist < AttackRange)
	{
		
		// Look at attacking target
		FRotator rotation = UZeroesHelper::LookAtTarget(this->GetActorLocation(), enemy->GetActorLocation(), this->GetActorRotation());
		this->SetActorRotation(rotation);

		if (enemy && enemy->GetHealth() > 0)
		{
			m_heroState->SetCanAttack(false);

			GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &AHeroBase::OnAttackCooldownFinished, AttackCooldown, false);
			UE_LOG(LogZeroes, Log, TEXT("Attacked '%s'. Cooling down for '%f' seconds"), *enemy->GetName(), AttackCooldown);
			
			if (AttackSound) 
				UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));

			if (enemy->GetHealth() <= 0)
			{
				// Enemy died on attack
				m_playerController->ResetTargetEnemy();
				// Reset state
				SetState(PlayerStates::IDLE);
			}
			else 
			{
				// Broadcast IsAttacking event
				if (OnBeginAttacking.IsBound())
					OnBeginAttacking.Broadcast();
			}
		}
	}
}

void AHeroBase::UseAbilityOnePressed()
{
	UseAbility(0);
}

void AHeroBase::UseAbilityTwoPressed()
{
	UseAbility(1);
}

void AHeroBase::UseAbilityThreePressed()
{
	UseAbility(2);
}

void AHeroBase::UseUltimatePressed()
{
	UseAbility(3);
}

void AHeroBase::UseCancelCommandPressed()
{
	UE_LOG(LogZeroes, Log, TEXT("Cancel pressed!"));
	if (State == PlayerStates::ATTACKING) 
	{
		CancelAttack();
	}
	else
	{
		m_playerController->CancelMovement();
	}
}

void AHeroBase::UseAbility(int index)
{
	if (m_heroState->GetAbilityCanUse(index))
	{
		UE_LOG(LogZeroes, Log, TEXT("Using Ability %d!"), (index + 1));
		m_heroState->SetAbilityCanUse(index, false);

		// Trigger Begin Ability event to any listeners
		if (OnBeginAbility.IsBound())
			OnBeginAbility.Broadcast(index);

		switch (index)
		{
			case 0:
				UseAbilityOne();
				GetWorldTimerManager().SetTimer(TimerHandle_AbilityOneCooldown, this, &AHeroBase::OnAbilOneCooldownFinished, AbilityOneCooldown, false);
				break;
			case 1:
				UseAbilityTwo();
				GetWorldTimerManager().SetTimer(TimerHandle_AbilityTwoCooldown, this, &AHeroBase::OnAbilTwoCooldownFinished, AbilityTwoCooldown, false);
				break;
			case 2:
				UseAbilityThree();
				GetWorldTimerManager().SetTimer(TimerHandle_AbilityThreeCooldown, this, &AHeroBase::OnAbilThreeCooldownFinished, AbilityThreeCooldown, false);
				break;
			case 3:
				UseUltimate();
				GetWorldTimerManager().SetTimer(TimerHandle_UltimateCooldown, this, &AHeroBase::OnUltimateCooldownFinished, UltimateCooldown, false);
				break;
			default:
				UE_LOG(LogZeroes, Error, TEXT("%s - No ability index found for '%f'"), *this->GetName(), index);
		}
	}
}

void AHeroBase::OnAbilOneCooldownFinished()
{
	m_heroState->SetAbilityCanUse(0, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability ONE"));

	// Broadcast ABilityComplete event
	if (OnCompleteAbility.IsBound())
		OnCompleteAbility.Broadcast(0);
}

void AHeroBase::OnAbilTwoCooldownFinished()
{
	m_heroState->SetAbilityCanUse(1, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability TWO"));

	if (OnCompleteAbility.IsBound())
		OnCompleteAbility.Broadcast(1);
}

void AHeroBase::OnAbilThreeCooldownFinished()
{
	m_heroState->SetAbilityCanUse(2, true);
	UE_LOG(LogZeroes, Log, TEXT("Can use Ability THREE"));

	if (OnCompleteAbility.IsBound())
		OnCompleteAbility.Broadcast(2);
}

void AHeroBase::OnUltimateCooldownFinished()
{
	m_heroState->SetAbilityCanUse(3, true);
	UE_LOG(LogZeroes, Log, TEXT("Ultimate cooldown expired. Can use ultimate"));

	if (OnCompleteAbility.IsBound())
		OnCompleteAbility.Broadcast(3);
}

void AHeroBase::OnAttackCooldownFinished()
{
	// Once attack cooldown is finished, enable attacking again
	m_heroState->SetCanAttack(true);

	if (OnCompletedAttacking.IsBound())
		OnCompletedAttacking.Broadcast();
}

void AHeroBase::HandleResetEngagement()
{
	UE_LOG(LogZeroes, Log, TEXT("Resetting Player enegagement - idle state"));
	SetState(PlayerStates::IDLE);
}

void AHeroBase::HandleStartMovement()
{
	SetState(PlayerStates::WALKING);
}

void AHeroBase::HandleEndedMovement()
{
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