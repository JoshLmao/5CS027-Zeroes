// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Zeroes.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = AAIController::StaticClass();

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	IdleRange = 750.0f;
	ChaseRange = 1000.0f;
	ChaseSpeed = 2.5f;
	AttackMinDistance = 150.0f;
	m_bCanAttack = false;
	AtkCooldownDuration = 5.0f;
	AttackDamage = 10.0f;
	Health = 1000.0f;

	// Set maximum movemenet speed
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_spawnLocation = this->GetActorLocation();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
		UE_LOG(LogZeroes, Error, TEXT("Minion has no reference to player!"));

	AIController = Cast<AAIController>(GetController());
	if (!AIController)
		UE_LOG(LogZeroes, Error, TEXT("No reference to AI Controller!"));
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FSMUpdate(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= damageValue;

	UE_LOG(LogZeroes, Log, TEXT("Enemy: Damage taken: %f - Health Remaining: %f"), damageValue, Health);
	return damageValue;
}

void AEnemyBase::FSMUpdate(float DeltaTime)
{
	switch (State)
	{
	case BehaviourStates::IDLE:
		if (Event == GameEvents::ON_START)
			IdleStart();
		else if (Event == GameEvents::ON_UPDATE)
			IdleUpdate();
		break;
	case BehaviourStates::CHASE:
		if (Event == GameEvents::ON_START)
			ChaseStart();
		else if (Event == GameEvents::ON_UPDATE)
			ChaseUpdate(DeltaTime);
		break;
	case BehaviourStates::ATTACK:
		if (Event == GameEvents::ON_START)
			AttackStart();
		else if (Event == GameEvents::ON_UPDATE)
			AttackUpdate();
		break;
	}
}

void AEnemyBase::SetState(BehaviourStates newState)
{
	State = newState;
	Event = GameEvents::ON_START;
}

void AEnemyBase::IdleStart()
{
	Event = GameEvents::ON_UPDATE;
}

void AEnemyBase::IdleUpdate()
{
	if (PlayerPawn)
	{
		float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());
		//UE_LOG(LogZeroes, Log, TEXT("Dist: %f - Range: %f"), distance, IdleRange);

		if (distance <= IdleRange)
		{
			UE_LOG(LogZeroes, Log, TEXT("Player in range, chasing!"));
			SetState(BehaviourStates::CHASE);
		}
	}
}

void AEnemyBase::ChaseStart()
{
	Event = GameEvents::ON_UPDATE;
}

void AEnemyBase::ChaseUpdate(float DeltaTime)
{
	if (PlayerPawn && AIController)
	{
		AIController->MoveToLocation(PlayerPawn->GetActorLocation(), 25.0f, true);
	}

	float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (distance <= AttackMinDistance)
	{
		// Stop movement to attack
		AIController->StopMovement();

		SetState(BehaviourStates::ATTACK);
		UE_LOG(LogZeroes, Log, TEXT("In range of player, attacking them"));
	}
	else if (distance >= ChaseRange)
	{
		// Return to spawn if lost player
		AIController->MoveToLocation(m_spawnLocation);

		if (this->GetActorLocation().Equals(m_spawnLocation))
		{
			// Returned home
			SetState(BehaviourStates::IDLE);
		}
	}
}

void AEnemyBase::AttackStart()
{
	Event = GameEvents::ON_UPDATE;
	m_bCanAttack = true;
}

void AEnemyBase::AttackUpdate()
{
	if (m_bCanAttack)
	{
		m_bCanAttack = false;

		GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &AEnemyBase::OnAtkCooldownFinished, AtkCooldownDuration);
		UE_LOG(LogZeroes, Log, TEXT("Attacked Player. Cooling down"));

		if (PlayerPawn)
		{
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			PlayerPawn->TakeDamage(AttackDamage, DamageEvent, nullptr, this);

			// Broadcast IsAttacking event
			if (OnMinionAttacking.IsBound())
				OnMinionAttacking.Broadcast();
		}
	}

	// Always look at player whilst waiting to attack
	LookAtTarget(PlayerPawn->GetActorLocation());

	float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (distance > AttackMinDistance)
	{
		SetState(BehaviourStates::CHASE);
		UE_LOG(LogZeroes, Log, TEXT("Player went out of range. Go bk to them"));
	}
}

void AEnemyBase::OnAtkCooldownFinished()
{
	m_bCanAttack = true;
	UE_LOG(LogZeroes, Log, TEXT("Cooldown finished. Can atk again"));
}

void AEnemyBase::LookAtTarget(FVector target)
{
	FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), target);
	FRotator currentRotation = this->GetActorRotation();
	currentRotation.Yaw = lookAtRotator.Yaw;
	SetActorRotation(currentRotation);
}
