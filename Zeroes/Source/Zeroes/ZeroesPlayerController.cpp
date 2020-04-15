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

AZeroesPlayerController::AZeroesPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	m_currentTargetVector = FVector::ZeroVector;

	m_reachedEnemy = false;
	m_targetEnemy = nullptr;
}

void AZeroesPlayerController::ResetTargetEnemy()
{
	m_targetEnemy = nullptr;
	m_reachedEnemy = false;
	UE_LOG(LogZeroes, Log, TEXT("ZeroesPlayerController: Reset Target Enemy"));
}

void AZeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (m_disableMovement)
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
		if (dist < 110.0f)
		{
			if (OnEndedMovement.IsBound()) {
				OnEndedMovement.Broadcast();
			}
			m_currentTargetVector = FVector::ZeroVector;
		}
	}
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
				SetNewMoveDestination(Hit.ImpactPoint);

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
		if ((Distance > rangeTolerance))
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

void AZeroesPlayerController::CancelMovement()
{
	if (!GetPawn()->GetVelocity().IsZero()) {
		StopMovement();
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


