// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

/**
 * Spike actor - Controls a spike for Daoko's ability one, which will rise 
 * from the ground and disappear after a duration. Also deals damage to any AEnemyBase
 * it collides with.
 */
UCLASS()
class ZEROES_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Overlap function to listen to capsule's OnComponentBeginOverlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// Gets the current damage the spike will deal
	float GetDamage();
	/// Sets the current damage the spike will deal
	void SetDamage(float damage);

	/// Get the total lifetime in seconds of the spike
	float GetLifeTime();
	/// Set the total lifetime in seconds of the spike
	void SetLifeTime(float lifeTimeSeconds);

private:
	/// Root component, capsule component
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* m_capsule;

	/// Visual mesh of the spike
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_mesh;

	/// Amount of units to sink the spike into the ground from it's spawn position
	const float SINK_AMOUNT = 100.0f;
	/// Amount of UE4 units to rise spike from the ground, after SINK_AMOUNT has been applied
	const float RISE_AMOUNT = 200.0f;

	/// Spike's vector to lerp to after it's rise animation
	FVector m_originalVector;
	/// Spike's riseFrom vector to lerp from
	FVector m_riseFromVector;
	/// Speed the spike will rise out of the ground
	float m_riseSpeed;

	/// Amount of damage the spike deals
	float m_damage;
	/// Lifetime in seconds of the spike
	float m_lifeTime;

	/// Called every frame to do the Rise animation
	void DoAnimation(float DeltaTime);
};
