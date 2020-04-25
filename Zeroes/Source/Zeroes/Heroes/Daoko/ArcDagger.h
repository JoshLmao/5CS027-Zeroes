// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcDagger.generated.h"

/** Arc Dagger Ability Actor
 * Determines the nearest AEnemyBase actor and flies toward it in an arc.
 */
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

	/// Overlap function to listen to the capsule component's OnComponentOverlapBegin
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// Gets current damage that collided enemy will recieve
	float GetDamage();
	// Sets the current damage to deal to collided enemy
	void SetDamage(float dmg);

private:
	/// Root component, capsule component
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* m_capsule;

	/// Visual mesh of the dagger
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_mesh;

	/// Amount of damage the dagger will give to the enemy
	float m_damage;
	/// Current target enemy
	class AEnemyBase* m_targetEnemy;
	/// Starting spawn position to arc from
	FVector m_startPosition;
	/// Position to arc out toward
	FVector m_arcToPosition;
	/// Current alpha value which determines the current position alog the arc
	float m_arcAlpha;

	/// Finds an enemy to arc toward and deal damage
	class AEnemyBase* FindEnemy();

	/** Gets the relevant position along a spline from the 'From' vector to the 'to' vector, 
	* using the 'arcTo' vector to control where to arc to
	*/
	FVector QuadraticGetPoint(float alpha, FVector from, FVector arcTo, FVector to);
};
