// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SActorWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API USActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	USActorWidgetComponent();

public:
	virtual void InitWidget() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
