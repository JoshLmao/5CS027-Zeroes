// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroesPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerReachedDestinationActorSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerResetEngagement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerStartMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerEndedMovement);

UCLASS()
class AZeroesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZeroesPlayerController();

	UPROPERTY(BlueprintAssignable)
	FControllerReachedDestinationActorSignature OnReachedDestActor;

	UPROPERTY(BlueprintAssignable)
	FControllerResetEngagement OnResetEngagement;

	UPROPERTY(BlueprintAssignable)
	FControllerStartMovement OnStartMovement;

	UPROPERTY(BlueprintAssignable)
	FControllerEndedMovement OnEndedMovement;

	void ResetTargetEnemy();

	void SetDisableMovement(bool disable);
	bool GetDisableMovement();

	/// Cancels the current movement command
	void CancelMovement();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation, float rangeTolerance = 70.0f);

	/** Navigate to enemy for combat */
	void SetNewEnemyDestination(AActor* actor);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();


private:
	AActor* m_targetEnemy;

	bool m_reachedEnemy;
	bool m_disableMovement;
	// Current vector the player is wanting to move to
	FVector m_currentTargetVector;
};

