// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SActorWidget.h"
#include "EnemyHealthbar.generated.h"

/**
 * 
 */
UCLASS()
class ZEROES_API UEnemyHealthbar : public USActorWidget
{
	GENERATED_BODY()
	
public:
	/// Current health of the enemy
	UPROPERTY(BlueprintReadOnly)
	float Health;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	float HealthPercent;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void SetOwningActor(AActor* NewOwner) override;
private:
	class AEnemyBase* m_enemy;
};
