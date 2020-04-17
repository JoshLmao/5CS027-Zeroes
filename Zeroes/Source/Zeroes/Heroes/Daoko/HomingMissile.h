// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingMissile.generated.h"

UCLASS()
class ZEROES_API AHomingMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// Gets the current damage of the missile
	float GetDamage();
	/// Sets the damage of the missile
	void SetDamage(float dmg);

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* m_capsule;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_mesh;

	FTimerHandle TimerHandle_SeekDelay;

	float m_damage;
	FVector m_riseStart;
	FVector m_riseTarget;
	float m_riseAlpha;
	AActor* m_targetEnemy;
	float m_missileMoveSpeed;
	FVector m_seekStart;
	float m_seekAlpha;

	class USoundBase* m_explosionSound;
	class UParticleSystem* m_explosionPS;

	enum BehaviourStates { RISE, SEARCH, SEEK };
	BehaviourStates State = BehaviourStates::RISE;

	enum StateEvents { START, UPDATE };
	StateEvents StateEvent = StateEvents::START;

	void FSM_Update(float DeltaTime);
	void SetState(BehaviourStates state);

	void RiseStart();
	void RiseUpdate(float DeltaTime);

	void SearchStart();
	void SearchUpdate();

	void SeekStart();
	void SeekUpdate(float DeltaTime);

	void OnSeekDelayComplete();

	void DestroyMissile();
};
