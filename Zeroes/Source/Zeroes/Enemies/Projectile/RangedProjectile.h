// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedProjectile.generated.h"

/**
*	Projectile of a ranged enemy to be fired
*/
UCLASS()
class ZEROES_API ARangedProjectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ARangedProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/// Gets the amount of damage the projectile will deal
	float GetDamage();
	/// Sets the amount of damage the projectile
	void SetDamage(float dmgAmount);

	/// Set the current fire direction of the projectile
	void FireInDirection(FVector direction);

	/// Sphere component collider to handle collision
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class USphereComponent* m_sphere;

private:
	/// Current projectile movement component on the projectile
	class UProjectileMovementComponent* m_projectileMovement;
	/// Amount of damage to deal to the player
	float m_damageAmount;
};
