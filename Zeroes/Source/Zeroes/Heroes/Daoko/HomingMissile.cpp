// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemies/EnemyBase.h"
#include "Zeroes.h"
#include "ConstructorHelpers.h"
#include "Kismet\KismetSystemLibrary.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles\ParticleSystem.h"
#include "Kismet\KismetMathLibrary.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
	m_missileMoveSpeed = 2.5f;
	m_riseTarget = FVector::ZeroVector;
	m_targetEnemy = nullptr;
	m_damage = 50.0f;
	m_riseAlpha = 0.0f;
	m_seekAlpha = 0.0f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	m_capsule->SetCapsuleHalfHeight(45.0f);
	m_capsule->SetCapsuleRadius(20.0f);
	// Enable collision and disable gravity
	m_capsule->SetNotifyRigidBodyCollision(true);
	m_capsule->SetEnableGravity(false);

	m_capsule->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_capsule->OnComponentBeginOverlap.AddDynamic(this, &AHomingMissile::OnOverlapBegin);

	RootComponent = m_capsule;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_mesh->SetupAttachment(RootComponent);
	m_mesh->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> missileMesh(TEXT("/Game/ParagonShinbi/FX/Meshes/Shapes/SM_Aura_v1"));
	if (missileMesh.Succeeded())
	{
		m_mesh->SetStaticMesh(missileMesh.Object);

		// Set mesh pos/rot/scale
		m_mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
		m_mesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		m_mesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25));
	}
	else
	{
		UE_LOG(LogZeroes, Error, TEXT("Unable to find mesh for Homing Missile!"));
	}

	// Load Explosion Sound
	static ConstructorHelpers::FObjectFinder<USoundBase> explosionSound(TEXT("/Game/Audio/missile_explosion"));
	if (explosionSound.Succeeded())
		m_explosionSound = explosionSound.Object;
	else
		UE_LOG(LogZeroes, Error, TEXT("No Explosion sound set on Missile!"));

	// Load explosion particle system 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> explosionPS(TEXT("/Game/Particles/PS_FireExplosion"));
	if (explosionPS.Succeeded())
		m_explosionPS = explosionPS.Object;
	else
		UE_LOG(LogZeroes, Error, TEXT("No Explosion PS set on Missile!"));
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FSM_Update(DeltaTime);
}

void AHomingMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyBase::StaticClass()))
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);

		const FDamageEvent dmgEvent;
		enemy->TakeDamage(m_damage, dmgEvent, nullptr, this);

		DestroyMissile();
	}
}

float AHomingMissile::GetDamage()
{
	return m_damage;
}

void AHomingMissile::SetDamage(float dmg)
{
	m_damage = dmg;
}

void AHomingMissile::FSM_Update(float DeltaTime)
{
	switch (State)
	{
		case BehaviourStates::RISE:
			if (StateEvent == StateEvents::START)
				RiseStart();
			if (StateEvent == StateEvents::UPDATE)
				RiseUpdate(DeltaTime);
			break;
		case BehaviourStates::SEARCH:
			if (StateEvent == StateEvents::START)
				SearchStart();
			if (StateEvent == StateEvents::UPDATE)
				SearchUpdate();
			break;
		case BehaviourStates::SEEK:
			if (StateEvent == StateEvents::START)
				SeekStart();
			if (StateEvent == StateEvents::UPDATE)
				SeekUpdate(DeltaTime);
			break;

	}
}

void AHomingMissile::SetState(BehaviourStates state)
{
	State = state;
	StateEvent = StateEvents::START;
}

void AHomingMissile::RiseStart()
{
	StateEvent = StateEvents::UPDATE;

	m_riseStart = GetActorLocation();
	m_riseTarget = GetActorLocation() + (GetActorUpVector() * 200.0f);
}

void AHomingMissile::RiseUpdate(float DeltaTime)
{
	m_riseAlpha += m_missileMoveSpeed * DeltaTime;

	SetActorLocation(FMath::Lerp(m_riseStart, m_riseTarget, m_riseAlpha));

	if (m_riseAlpha >= 1.0f) {
		SetState(BehaviourStates::SEARCH);
	}
}

void AHomingMissile::SearchStart()
{
	StateEvent = StateEvents::UPDATE;

	float delay = 2.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_SeekDelay, this, &AHomingMissile::OnSeekDelayComplete, delay, false);
}

void AHomingMissile::SearchUpdate()
{
}

void AHomingMissile::SeekStart()
{
	StateEvent = StateEvents::UPDATE;
	m_seekStart = GetActorLocation();
}

void AHomingMissile::SeekUpdate(float DeltaTime)
{
	if (m_targetEnemy)
	{
		if (m_seekAlpha < 1.0f)
		{
			m_seekAlpha += m_missileMoveSpeed * DeltaTime;
		}

		FVector position = FMath::Lerp(m_seekStart, m_targetEnemy->GetActorLocation(), m_seekAlpha);
		SetActorLocation(position);

		FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_targetEnemy->GetActorLocation());
		lookAtRotator.Pitch -= 90.0f;
		SetActorRotation(lookAtRotator);
	}
}

void AHomingMissile::OnSeekDelayComplete()
{
	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	// Total radius of the sphere
	float radius = 1000.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = AEnemyBase::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	// Optional: Use to have a visual representation of the SphereOverlapActors
	//DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, false, 5.0f);
	
	// Determine what actor/pawn to seek
	float distance = 0.0f;
	for (AActor* overlappedActor : outActors)
	{
		UE_LOG(LogZeroes, Log, TEXT("Missile OverlappedActor: %s"), *overlappedActor->GetName());

		// Target any actor that isn't a missile
		if (!overlappedActor->IsA(AHomingMissile::StaticClass()))
		{
			// Determine the target by the closest actor
			float currentDist = FVector::Dist(GetActorLocation(), overlappedActor->GetActorLocation());
			if (distance == 0.0f || currentDist < distance)
				m_targetEnemy = overlappedActor;
		}
	}

	if (m_targetEnemy != nullptr)
	{
		SetState(BehaviourStates::SEEK);
		UE_LOG(LogZeroes, Log, TEXT("Seeking to Actor '%s'"), *m_targetEnemy->GetName());
	}
	else
	{
		UE_LOG(LogZeroes, Warning, TEXT("Unable to find an enemy to seek to on '%s'"), *GetName());
		DestroyMissile();
	}
}

void AHomingMissile::DestroyMissile()
{
	if (m_explosionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_explosionSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));
		

	if (m_explosionPS)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_explosionPS, GetActorLocation());

	Destroy();
}

