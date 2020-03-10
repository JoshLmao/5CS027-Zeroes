// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemies\EnemyBase.h"
#include "Zeroes.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpike::ASpike()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_damage = 50.0f;
	m_lifeTime = 5.0f;
	m_riseSpeed = 10.0f;
	m_originalVector = m_riseFromVector = FVector::ZeroVector;

	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	m_capsule->SetCapsuleHalfHeight(135.0f);
	m_capsule->SetCapsuleRadius(75.0f);
	
	m_capsule->SetNotifyRigidBodyCollision(true);
	m_capsule->SetEnableGravity(false);

	m_capsule->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_capsule->BodyInstance.bLockRotation = true;
	m_capsule->BodyInstance.bLockXRotation = true;
	m_capsule->BodyInstance.bLockYRotation = true;
	m_capsule->BodyInstance.bLockZRotation = true;
	
	m_capsule->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnOverlapBegin);

	RootComponent = m_capsule;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spike Mesh"));
	m_mesh->SetupAttachment(RootComponent);
	m_mesh->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RockMesh(TEXT("StaticMesh'/Game/ParagonGrux/FX/Meshes/Debris/SM_Rock_02.SM_Rock_02'"));
	if (RockMesh.Succeeded())
	{
		m_mesh->SetStaticMesh(RockMesh.Object);

		m_mesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
		m_mesh->SetRelativeScale3D(FVector(1.5f, 0.75f, 1.0f));
	}
	else
		UE_LOG(LogZeroes, Error, TEXT("Unable to set mesh for Spike! Can't find Rock"));
}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(m_lifeTime);

	// Get start and riseFrom values
	m_originalVector = this->GetActorLocation();
	// Variate the Z position of a spike
	float variance = 45.0f;
	m_originalVector.Z = FMath::RandRange(m_originalVector.Z - variance, m_originalVector.Z + variance);
	m_riseFromVector = m_originalVector + FVector(0.0f, 0.0f, -200.0f);
	this->SetActorLocation(m_riseFromVector);

	// Set random yaw rotation
	float yaw = FMath::RandRange(0.0f, 360.0f);
	this->SetActorRelativeRotation(FRotator(0.0f, yaw, 0.0f));
}

// Called every frame
void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoAnimation(DeltaTime);
}

void ASpike::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyBase::StaticClass()))
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);
		if (enemy->Health > 0)
		{
			UE_LOG(LogZeroes, Log, TEXT("Spike hit enemy! Dealing damage %f"), m_damage);

			const FDamageEvent dmgEvent;
			OtherActor->TakeDamage(m_damage, dmgEvent, nullptr, this);

			Destroy();
		}
	}
}

float ASpike::GetDamage()
{
	return m_damage;
}

void ASpike::SetDamage(float damage)
{
	m_damage = damage;
}

float ASpike::GetLifeTime()
{
	return m_lifeTime;
}

void ASpike::SetLifeTime(float lifeTimeSeconds)
{
	m_lifeTime = lifeTimeSeconds;
}

void ASpike::DoAnimation(float DeltaTime)
{
	FVector vectorToTarget = m_originalVector - this->GetActorLocation();
	vectorToTarget.Normalize();
	vectorToTarget *= m_riseSpeed;

	// Update position and look at target
	this->SetActorLocation(this->GetActorLocation() + vectorToTarget);
}

