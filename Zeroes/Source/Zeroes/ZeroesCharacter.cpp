// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Zeroes.h"
#include "Enemies\EnemyBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ZeroesPlayerController.h"

AZeroesCharacter::AZeroesCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		decalDynamicMaterial = UMaterialInstanceDynamic::Create(DecalMaterialAsset.Object, NULL);
		CursorToWorld->SetDecalMaterial(decalDynamicMaterial);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AZeroesCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	CheckForBlockingActor();
	
	// Sets the location/rotation of the mouse to ground indicator
	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			

			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			// If cursor is out of window
			if (CursorFV.Equals(FVector::ZeroVector)) 
			{
				CursorToWorld->SetWorldLocation(FVector(99999, 99999, 99999));
			}
			else 
			{
				CursorToWorld->SetWorldLocation(TraceHitResult.Location);
				CurrentMouseWorldPosition = TraceHitResult.Location;
			}
			
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AZeroesCharacter::SetPreventMovement(bool shouldPreventMovement)
{
	AZeroesPlayerController* pc = Cast<AZeroesPlayerController>(GetController());
	pc->SetDisableMovement(shouldPreventMovement);
	
	if (shouldPreventMovement)
		GetController()->StopMovement();
}

void AZeroesCharacter::CheckForBlockingActor()
{
	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult Hit;
			PC->GetHitResultUnderCursor(ECC_Pawn, true, Hit);

			if (Hit.bBlockingHit)
			{
				AActor* actor = Hit.GetActor();
				FLinearColor color = FLinearColor::Green;
				if (actor && actor->IsA(AEnemyBase::StaticClass()))
				{
					//UE_LOG(LogZeroes, Log, TEXT("Hovering over an enemy"));
					color = FLinearColor::Red;
					DestinationActor = actor;
				}
				
				decalDynamicMaterial->SetVectorParameterValue(TEXT("DecalColor"), color);
				CursorToWorld->SetDecalMaterial(decalDynamicMaterial);
			}
		}
	}
}
