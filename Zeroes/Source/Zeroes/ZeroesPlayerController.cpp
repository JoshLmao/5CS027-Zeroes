// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZeroesPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ZeroesCharacter.h"
#include "Engine/World.h"
#include "Enemies\EnemyBase.h"
#include "Zeroes.h"
#include "GameFramework/Pawn.h"
#include "Heroes\HeroAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Heroes\HeroBase.h"
#include "Heroes/HeroState.h"
#include "DrawDebugHelpers.h"

AZeroesPlayerController::AZeroesPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	m_currentTargetVector = FVector::ZeroVector;

	m_reachedEnemy = false;
	m_targetEnemy = nullptr;
	m_destinationTolerance = 100.0f;
}

void AZeroesPlayerController::ResetTargetEnemy()
{
	m_targetEnemy = nullptr;
	m_reachedEnemy = false;
	UE_LOG(LogZeroes, Log, TEXT("ZeroesPlayerController: Reset Target Enemy"));
}

void AZeroesPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AHeroBase* hero = Cast<AHeroBase>(GetPawn());
	if (hero)
	{
		hero->OnCancelAttacking.AddDynamic(this, &AZeroesPlayerController::OnCancelAttacking);
	}
}

void AZeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AHeroBase* hero = Cast<AHeroBase>(GetPawn());
	if (m_disableMovement || (hero && hero->IsDead()))
		return;

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor) {
		MoveToMouseCursor();
	}

	// Update every tick to get the latest enemy position (could be moving)
	if (m_targetEnemy != nullptr  && !m_reachedEnemy)
	{
		SetNewEnemyDestination(m_targetEnemy);
	}

	/// Check for when SimpleMoveToLocation() has reached its destination
	if (m_currentTargetVector != FVector::ZeroVector)
	{
		float dist = FVector::Dist(GetPawn()->GetActorLocation(), m_currentTargetVector);
		//UE_LOG(LogZeroes, Log, TEXT("Distance: %f - Tolerance: %f Position: %s"), dist, m_destinationTolerance + 15.0f, *m_currentTargetVector.ToString());
		
		// Add a little extra for breathing room and error
		if (dist <= m_destinationTolerance + 15.0f)
		{
			if (OnEndedMovement.IsBound()) {
				OnEndedMovement.Broadcast();
			}
			m_currentTargetVector = FVector::ZeroVector;
		}
	}

	// Debug: Draw destination point
	//if (m_currentTargetVector != FVector::ZeroVector)
	//	DrawDebugSphere(GetWorld(), m_currentTargetVector, 25.0f, 10, FColor::Red);
}

void AZeroesPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AZeroesPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AZeroesPlayerController::OnSetDestinationReleased);
}

void AZeroesPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);

	USkeletalMeshComponent* mesh = Cast<USkeletalMeshComponent>(GetPawn()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	UHeroAnimInstance* animInstance = Cast<UHeroAnimInstance>(mesh->GetAnimInstance());

	if (Hit.bBlockingHit)
	{
		// Check if hovering over enemy
		AActor* actor = Hit.GetActor();
		if (!actor) {
			return;
		}

		if (actor->IsA(AEnemyBase::StaticClass()))
		{
			// Set target enemy that player wants to kill
			m_targetEnemy = actor;
		}
		else if (!actor->IsA(APawn::StaticClass()))
		{
			if (animInstance && !animInstance->bIsAttacking)
			{
				// Move to floor location
				SetNewMoveDestination(Hit.ImpactPoint, m_destinationTolerance);

				if (m_targetEnemy != nullptr)
				{
					// Reset tracked enemy on issuing a walk command
					if (OnResetEngagement.IsBound())
						OnResetEngagement.Broadcast();
					ResetTargetEnemy();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogZeroes, Warning, TEXT("Nothing blocking MoveTo hit result"));
	}
}

void AZeroesPlayerController::SetNewMoveDestination(const FVector DestLocation, float rangeTolerance)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (Distance >= rangeTolerance)
		{
			if (m_currentTargetVector == FVector::ZeroVector) {
				if (OnStartMovement.IsBound()) {
					OnStartMovement.Broadcast();
				}
			}

			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
			m_currentTargetVector = DestLocation;
		}
		else {
			if (m_currentTargetVector != FVector::ZeroVector) {
				if (OnEndedMovement.IsBound()) {
					OnEndedMovement.Broadcast();
				}
			}

			m_currentTargetVector = FVector::ZeroVector;
		}
	}
}

void AZeroesPlayerController::SetNewEnemyDestination(AActor* enemyActor)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(enemyActor);
	APawn* const MyPawn = GetPawn();
	float attackRange = Cast<AHeroBase>(MyPawn)->AttackRange;

	if (MyPawn)
	{
		float const Distance = FVector::Dist(enemy->GetActorLocation(), MyPawn->GetActorLocation());
		if (Distance > attackRange)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, enemy->GetActorLocation());
			m_reachedEnemy = false;

			if (OnTravelDestActor.IsBound())
				OnTravelDestActor.Broadcast();
		}
		else
		{
			if (!m_reachedEnemy)
			{
				//UE_LOG(LogZeroes, Log, TEXT("Controller reached enemy. Trigger event"))
				if (OnReachedDestActor.IsBound())
					OnReachedDestActor.Broadcast();
				m_reachedEnemy = true;
			}
		}
	}
}

void AZeroesPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AZeroesPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AZeroesPlayerController::OnCancelAttacking()
{
	ResetTargetEnemy();
}

void AZeroesPlayerController::CancelMovement()
{
	if (!GetPawn()->GetVelocity().IsZero()) 
	{
		StopMovement();
		if (OnEndedMovement.IsBound())
			OnEndedMovement.Broadcast();

		m_currentTargetVector = FVector::ZeroVector;
	}
}

void AZeroesPlayerController::SetDisableMovement(bool disable)
{
	m_disableMovement = disable;
}

bool AZeroesPlayerController::GetDisableMovement()
{
	return m_disableMovement;
}


