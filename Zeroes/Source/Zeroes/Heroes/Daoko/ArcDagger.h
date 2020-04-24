// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcDagger.generated.h"

UCLASS()
class ZEROES_API AArcDagger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArcDagger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/// Speed to go around the arc
	UPROPERTY(EditAnywhere)
	float ArcSpeed;
	/// Maximum ability distance
	float MaxDistance;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// Gets current damage that collided enemy will recieve
	float GetDamage();
	// Sets the current damage to deal to collided enemy
	void SetDamage(float dmg);

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* m_capsule;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_mesh;

	float m_damage;
	class AEnemyBase* m_targetEnemy;
	FVector m_startPosition;
	FVector m_arcToPosition;
	float m_arcAlpha;

	/// Finds an enemy to arc toward and deal damage
	class AEnemyBase* FindEnemy();

	FVector QuadraticGetPoint(float alpha, FVector from, FVector arcTo, FVector to);
};
