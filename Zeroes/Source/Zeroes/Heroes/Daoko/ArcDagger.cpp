// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcDagger.h"

#include "ConstructorHelpers.h"
#include "Components\StaticMeshComponent.h"
#include "Components\CapsuleComponent.h"
#include "Zeroes.h"
#include "Enemies\EnemyBase.h"
#include "EngineUtils.h"
#include "ZeroesMathHelper.h"
#include "DrawDebugHelpers.h"
#include "ZeroesHelper.h"
#include "Kismet\KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AArcDagger::AArcDagger()
{
	ArcSpeed = 5.0f;
	MaxDistance = 2500.0f;
	m_arcAlpha = 0.01f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	m_capsule->SetCapsuleHalfHeight(40.0f);
	m_capsule->SetCapsuleRadius(20.0f);
	m_capsule->SetNotifyRigidBodyCollision(true);
	m_capsule->SetEnableGravity(false);
	
	m_capsule->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_capsule->OnComponentBeginOverlap.AddDynamic(this, &AArcDagger::OnOverlapBegin);

	RootComponent = m_capsule;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_mesh->SetupAttachment(RootComponent);
	m_mesh->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> daggerMesh(TEXT("/Game/ParagonShinbi/FX/Meshes/Hero_Specific/SM_Shinbi_Sword_Surround"));
	if (daggerMesh.Succeeded())
	{
		m_mesh->SetStaticMesh(daggerMesh.Object);

		// Set relative pos/rot/scale
		m_mesh->SetRelativeLocation(FVector(0.000000, 0.000000, -50.000000));
		m_mesh->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
		m_mesh->SetRelativeScale3D(FVector(3.0f, 1.0f, 3.0f));
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to find mesh for %s"), *this->GetName());
	}
}

// Called when the game starts or when spawned
void AArcDagger::BeginPlay()
{
	Super::BeginPlay();
	
	AEnemyBase* enemy = FindEnemy();
	if (enemy)
	{
		UE_LOG(LogZeroes, Log, TEXT("Arc Dagger towards '%s' at location '%s'"), *enemy->GetName(), *enemy->GetActorLocation().ToString());

		m_startPosition = GetActorLocation();
		m_targetEnemy = enemy;

		FRotator rotation = UZeroesHelper::LookAtTarget(this->GetActorLocation(), m_targetEnemy->GetActorLocation(), this->GetActorRotation());
		this->SetActorRotation(rotation);

	}
	else
	{
		UE_LOG(LogZeroes, Log, TEXT("Arc unable to find enemy - Destroying"));
		m_targetEnemy = nullptr;
		Destroy();
	}
}

// Called every frame
void AArcDagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_targetEnemy)
	{
		// Increment alpha every tick
		float increment = 0.1f * ArcSpeed;
		m_arcAlpha += increment * DeltaTime;

		// Set after rotation has been set in beginplay
		if (m_arcToPosition.Equals(FVector::ZeroVector)) {
			m_arcToPosition = UZeroesMathHelper::GetAnyPointAlongLine(m_startPosition, m_targetEnemy->GetActorLocation(), 0.5f) + (GetActorRightVector() * 500.0f);
			
			DrawDebugSphere(GetWorld(), m_arcToPosition, 20.0f, 20.0f, FColor::Blue, true, -1.0f, 0, 1.0f);
		}

		// Set dagger to arc towards target using quadratic bezier
		FVector point = QuadraticGetPoint(m_arcAlpha, m_startPosition, m_arcToPosition, m_targetEnemy->GetActorLocation());
		SetActorLocation(point);

		// Setup actor to look toward target
		FVector loc = GetActorLocation();
		// Check enemy is still valid (otherwise crash if tick is midway on collision destroy)
		FVector targetLoc = m_targetEnemy ? m_targetEnemy->GetActorLocation() : FVector::ZeroVector;
		if (!loc.Equals(FVector::ZeroVector) && !targetLoc.Equals(FVector::ZeroVector)) 
		{
			FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(loc, targetLoc);
			// Add rotation offset to pitch
			lookAtRotator.Pitch += -90.0f;
			this->SetActorRotation(lookAtRotator);
		}

		// Debug Spheres for From Location, Arc Point and To Location
		//DrawDebugSphere(GetWorld(), m_startPosition, 20.0f, 20.0f, FColor::Red, false, -1.0f, 0, 1.0f);
		//DrawDebugSphere(GetWorld(), m_targetEnemy->GetActorLocation(), 20.0f, 20.0f, FColor::Red, false, -1.0f, 0, 1.0f);
		//DrawDebugSphere(GetWorld(), m_arcToPosition, 20.0f, 20.0f, FColor::Blue, false, -1.0f, 0, 1.0f);
	}
}

void AArcDagger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyBase::StaticClass()))
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);

		const FDamageEvent dmgEvent;
		enemy->TakeDamage(m_damage, dmgEvent, nullptr, this);

		UE_LOG(LogZeroes, Log, TEXT("%s: Dealt '%f' damage to %s, destroying!"), *this->GetName(), m_damage, *enemy->GetName());
		m_targetEnemy = nullptr;

		Destroy();
	}
}

float AArcDagger::GetDamage()
{
	return m_damage;
}

void AArcDagger::SetDamage(float dmg)
{
	m_damage = dmg;
}

AEnemyBase* AArcDagger::FindEnemy()
{
	AEnemyBase* closestEnemy = nullptr;
	float closestDistance = MaxDistance;
	for (TActorIterator<AEnemyBase> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		AEnemyBase* enemy = *TargetIter;
		float distance = FVector::Distance(this->GetActorLocation(), enemy->GetActorLocation());
		if (enemy && distance < closestDistance)
		{
			closestDistance = distance;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;
}

FVector AArcDagger::QuadraticGetPoint(float alpha, FVector from, FVector arcTo, FVector to)
{
	float resultX = FMath::Pow((1 - alpha), 2) * from.X + 2 * (1 - alpha) * alpha * arcTo.X + FMath::Pow(alpha, 2) * to.X;
	float resultY = FMath::Pow((1 - alpha), 2) * from.Y + 2 * (1 - alpha) * alpha * arcTo.Y + FMath::Pow(alpha, 2) * to.Y;
	//float resultZ = FMath::Pow((1 - alpha), 2) * from.Z + 2 * (1 - alpha) * alpha * arcTo.Z + FMath::Pow(alpha, 2) * to.Z;

	FVector currentTargetPosition(0, 0, 0);
	currentTargetPosition.X = resultX;
	currentTargetPosition.Y = resultY;
	currentTargetPosition.Z = to.Z; // Keep z the same height.

	return currentTargetPosition;
}
