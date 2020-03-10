// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

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

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float GetDamage();
	void SetDamage(float damage);

	float GetLifeTime();
	void SetLifeTime(float lifeTimeSeconds);

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* m_capsule;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_mesh;

	FVector m_originalVector;
	FVector m_riseFromVector;
	float m_riseSpeed;

	float m_damage;
	float m_lifeTime;

	void DoAnimation(float DeltaTime);
};
