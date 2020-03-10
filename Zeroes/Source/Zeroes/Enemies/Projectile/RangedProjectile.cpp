// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Heroes\HeroBase.h"
#include "Zeroes.h"

// Sets default values
ARangedProjectile::ARangedProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default value
	m_damageAmount = 100.0f;

	m_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_sphere->InitSphereRadius(5.0f);
	m_sphere->BodyInstance.SetCollisionProfileName("Projectile");
	m_sphere->OnComponentHit.AddDynamic(this, &ARangedProjectile::OnHit);

	// Players can't walk on it
	m_sphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	m_sphere->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = m_sphere;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	m_projectileMovement->UpdatedComponent = m_sphere;
	m_projectileMovement->InitialSpeed = 3000.f;
	m_projectileMovement->MaxSpeed = 3000.f;
	m_projectileMovement->bRotationFollowsVelocity = true;
	m_projectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ARangedProjectile::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(3.0f);
}

// Called every frame
void ARangedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangedProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		FDamageEvent dmgEvent;
		OtherActor->TakeDamage(this->GetDamage(), dmgEvent, nullptr, this);

		Destroy();
	}
}

float ARangedProjectile::GetDamage()
{
	return m_damageAmount;
}

void ARangedProjectile::SetDamage(float dmgAmount)
{
	m_damageAmount = dmgAmount;
}

void ARangedProjectile::FireInDirection(FVector direction)
{
	m_projectileMovement->Velocity = direction * m_projectileMovement->InitialSpeed;
}

