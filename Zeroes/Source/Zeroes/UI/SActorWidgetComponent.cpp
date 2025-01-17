// Fill out your copyright notice in the Description page of Project Settings.


#include "SActorWidgetComponent.h"
#include "SActorWidget.h"
#include "Zeroes.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/SActorWidget.h"

USActorWidgetComponent::USActorWidgetComponent()
{
	// Set common defaults when using widgets on Actors
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USActorWidgetComponent::InitWidget()
{
	// Base implementation creates the 'Widget' instance
	Super::InitWidget();

	if (Widget)
	{
#if !UE_BUILD_SHIPPING
		if (!Widget->IsA(USActorWidget::StaticClass()))
		{
			// Suggest deriving from actor widget (not strictly required, but you lose the benefit of auto-assigning the Actor this way)
			UE_LOG(LogZeroes, Warning, TEXT("WidgetClass for %s does not derive from SActorWidget"), *GetNameSafe(GetOwner()));
		}
#endif
		USActorWidget* WidgetInst = Cast<USActorWidget>(Widget);
		if (WidgetInst)
		{
			// Assign the owner, now we have easy access in the widget itself
			WidgetInst->SetOwningActor(GetOwner());
		}
	}
}
