// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedProjectile.generated.h"

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

	float GetDamage();
	void SetDamage(float dmgAmount);

	void FireInDirection(FVector direction);

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class USphereComponent* m_sphere;

private:

	class UProjectileMovementComponent* m_projectileMovement;

	float m_damageAmount;
};
