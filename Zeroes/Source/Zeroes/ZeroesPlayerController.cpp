// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZeroesPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ZeroesCharacter.h"
#include "Engine/World.h"
#include "Enemies\EnemyBase.h"
#include "Zeroes.h"

AZeroesPlayerController::AZeroesPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	m_reachedEnemy = false;
	m_targetEnemy = nullptr;
}

void AZeroesPlayerController::ResetTargetEnemy()
{
	m_targetEnemy = nullptr;
	m_reachedEnemy = false;
	UE_LOG(LogZeroes, Log, TEXT("Reset Target Enemy"));
}

void AZeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	// Update every tick to get the latest enemy position (could be moving)
	if (m_targetEnemy != nullptr  && !m_reachedEnemy)
	{
		SetNewEnemyDestination(m_targetEnemy);
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

	if (Hit.bBlockingHit)
	{
		// Check if hovering over enemy
		AActor* actor = Hit.GetActor();
		if (actor->IsA(AEnemyBase::StaticClass()))
		{
			// Set target enemy that player wants to kill
			m_targetEnemy = actor;
		}
		else
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
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}

void AZeroesPlayerController::SetNewEnemyDestination(AActor* enemyActor)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(enemyActor);
	float range = 175.0f;
	
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(enemy->GetActorLocation(), MyPawn->GetActorLocation());
		
		if (Distance > range)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, enemy->GetActorLocation());
			m_reachedEnemy = false;
		}
		else
		{
			if (!m_reachedEnemy)
			{
				//UE_LOG(LogZeroes, Log, TEXT("Controller reached enemy. Trigger event"))
				if (OnReachedDestActor.IsBound())
					OnReachedDestActor.Broadcast();
			}
			m_reachedEnemy = true;
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



