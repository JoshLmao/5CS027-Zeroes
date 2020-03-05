// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroesPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerReachedDestinationActorSignature);

UCLASS()
class AZeroesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZeroesPlayerController();

	UPROPERTY(BlueprintAssignable)
	FControllerReachedDestinationActorSignature OnReachedDestActor;

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
};


