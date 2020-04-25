// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroesPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerTravelToDestinationActorSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerReachedDestinationActorSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerResetEngagement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerStartMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerEndedMovement);

/*
* Main player controller of the player
*/
UCLASS()
class AZeroesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZeroesPlayerController();

	/// Travel to a destination event
	UPROPERTY(BlueprintAssignable)
	FControllerTravelToDestinationActorSignature OnTravelDestActor;
	
	/// Controller has reached it's destination
	UPROPERTY(BlueprintAssignable)
	FControllerReachedDestinationActorSignature OnReachedDestActor;

	/// Controller's combat engagement has been reset
	UPROPERTY(BlueprintAssignable)
	FControllerResetEngagement OnResetEngagement;

	/// Controller begins it's movement
	UPROPERTY(BlueprintAssignable)
	FControllerStartMovement OnStartMovement;

	/// Controller has ended it's movement
	UPROPERTY(BlueprintAssignable)
	FControllerEndedMovement OnEndedMovement;

	/// Resets the current controller's target enemy
	void ResetTargetEnemy();

	/// Sets if to disable movement on the current controller
	void SetDisableMovement(bool disable);
	/// Gets the current state of if the controller should block movement
	bool GetDisableMovement();

	/// Cancels the current movement command
	void CancelMovement();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation, float rangeTolerance = 100.0f);

	/** Navigate to enemy for combat */
	void SetNewEnemyDestination(AActor* actor);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

private:
	/// Current target enemy the controller is targeting
	AActor* m_targetEnemy;
	/// Has the controller reached it's current target enemy
	bool m_reachedEnemy;
	/// Is movement currently disabled for the controller
	bool m_disableMovement;
	// Current vector the player is wanting to move to
	FVector m_currentTargetVector;
	/// Amount of units of tolerance to use for the controller to 'arrive' at it's destination
	float m_destinationTolerance;

	/// Handles when attacking is cancelled by the player
	UFUNCTION()
	void OnCancelAttacking();
};

