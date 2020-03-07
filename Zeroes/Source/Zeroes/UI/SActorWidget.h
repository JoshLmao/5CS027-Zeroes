// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SActorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API USActorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/* Actor that widget is attached to via WidgetComponent */
	UPROPERTY(BlueprintReadOnly, Category = "ActorWidget")
	AActor* OwningActor;

public:
	/* Set the owning actor so widgets have access to whatever is, broadcasting OnOwningActorChanged event */
	UFUNCTION(BlueprintCallable, Category = "LODZERO|UI")
	virtual void SetOwningActor(AActor* NewOwner);
};
