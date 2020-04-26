// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Zeroes.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Components\SkeletalMeshComponent.h"
#include "Enemies\EnemyAnimInstance.h"
#include "UI\SActorWidget.h"
#include "UI/SActorWidgetComponent.h"
#include "ConstructorHelpers.h"
#include "UI/Enemies/EnemyHealthbar.h"
#include "ZeroesHelper.h"
#include "Heroes\HeroState.h"
#include "Heroes/HeroBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = AAIController::StaticClass();

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	Health = MaxHealth = 250.0f;
	IdleRange = 750.0f;
	ChaseRange = 1000.0f;
	ChaseSpeed = 2.5f;
	AttackMinDistance = 150.0f;
	bCanPerformAttack = false;
	m_bDeathTimedOut = false;
	AtkCooldownDuration = 5.0f;
	AttackDamage = 15.0f;
	m_deathSinkRate = 10.0f;
	AttackCount = 0;

	// Set maximum movemenet speed
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	//GetCapsuleComponent()->SetMassScale(NAME_None, 1000.0f);
	//GetMesh()->SetMassScale(NAME_None, 1000.0f);

	WidgetComponent = CreateDefaultSubobject<USActorWidgetComponent>("PopupWidget");
	WidgetComponent->SetupAttachment(RootComponent);
	// Always face camera by setting to screen
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// Set position to be above enemy head 
	WidgetComponent->SetRelativeLocationAndRotation(FVector(0, 0, 100.0f), FQuat::Identity);
	WidgetComponent->SetTickWhenOffscreen(true);

	HealthbarWidget = UEnemyHealthbar::StaticClass();

	static ConstructorHelpers::FObjectFinder<USoundBase> attackSound(TEXT("/Game/Audio/dk_sword_impact"));
	if (attackSound.Succeeded())
		AttackSound = attackSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> deathSound(TEXT("/Game/Audio/Robot_1"));
	if (deathSound.Succeeded())
		DeathSound = deathSound.Object;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_spawnLocation = this->GetActorLocation();

	PlayerPawn = Cast<AHeroBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!PlayerPawn)
		UE_LOG(LogZeroes, Error, TEXT("Minion has no reference to player!"));

	AIController = Cast<AAIController>(GetController());
	if (!AIController)
		UE_LOG(LogZeroes, Error, TEXT("No reference to AI Controller!"));

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance)
		UE_LOG(LogZeroes, Error, TEXT("No reference to an EnemyAnimInstance on enemy '%s'"), *this->GetName());

	// Assign blueprint to component
	WidgetComponent->SetWidgetClass(HealthbarWidget.Get());
	// Set wanted size of healthbar
	WidgetComponent->SetDrawSize(FVector2D(160, 20));
}

void AEnemyBase::OnAttack(AActor* attackEnemy)
{
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	PlayerPawn->TakeDamage(AttackDamage, DamageEvent, nullptr, this);

	if (AttackSound)
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), 1.0f, FMath::RandRange(0.75f, 1.25f));
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FSMUpdate(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= damageValue;
	
	if (Health <= 0)
	{
		OnDeath();
	}
	else
	{
		UE_LOG(LogZeroes, Log, TEXT("%s: Damage taken: %f - Health Remaining: %f"), *this->GetName(), damageValue, Health);
	}

	return damageValue;
}

float AEnemyBase::GetHealth()
{
	return Health;
}

float AEnemyBase::GetMaxHealth()
{
	return MaxHealth;
}

int AEnemyBase::GetAttackCount()
{
	return AttackCount;
}

void AEnemyBase::DealDamageToTarget()
{
	if (PlayerPawn)
	{
		OnAttack(PlayerPawn);
		AttackCount++;
	}
}

void AEnemyBase::Notify_FinishedAttackAnim()
{
	UEnemyAnimInstance* animInstance = this->AnimInstance;
	if (animInstance)
	{
		animInstance->OnAttackComplete();
	}
}

void AEnemyBase::FSMUpdate(float DeltaTime)
{
	switch (State)
	{
		case EBehaviourStates::IDLE:
			if (Event == GameEvents::ON_START)
				IdleStart();
			else if (Event == GameEvents::ON_UPDATE)
				IdleUpdate();
			break;
		case EBehaviourStates::CHASE:
			if (Event == GameEvents::ON_START)
				ChaseStart();
			else if (Event == GameEvents::ON_UPDATE)
				ChaseUpdate(DeltaTime);
			break;
		case EBehaviourStates::ATTACK:
			if (Event == GameEvents::ON_START)
				AttackStart();
			else if (Event == GameEvents::ON_UPDATE)
				AttackUpdate();
			break;
		case EBehaviourStates::DEAD:
			if (Event == GameEvents::ON_START)
				DeadStart();
			else if (Event == GameEvents::ON_UPDATE)
				DeadUpdate();
			break;
	}
}

void AEnemyBase::SetState(EBehaviourStates newState)
{
	State = newState;
	Event = GameEvents::ON_START;
}

void AEnemyBase::OnDeath()
{
	UE_LOG(LogZeroes, Log, TEXT("No health remaining on enemy '%s'. Dying"), *this->GetName());
	SetState(EBehaviourStates::DEAD);

	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), 1.0f, FMath::RandRange(0.8f, 1.2f));

	if (OnEnemyDeath.IsBound())
		OnEnemyDeath.Broadcast();
}

void AEnemyBase::IdleStart()
{
	Event = GameEvents::ON_UPDATE;
}

void AEnemyBase::IdleUpdate()
{
	if (PlayerPawn && !Cast<AHeroBase>(PlayerPawn)->bIsDead)
	{
		float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());

		if (distance <= IdleRange)
		{
			UE_LOG(LogZeroes, Log, TEXT("Player in range, chasing!"));
			SetState(EBehaviourStates::CHASE);
		}
	}
}

void AEnemyBase::ChaseStart()
{
	Event = GameEvents::ON_UPDATE;
}

void AEnemyBase::ChaseUpdate(float DeltaTime)
{
	if (PlayerPawn && AIController)
	{
		AIController->MoveToLocation(PlayerPawn->GetActorLocation(), 25.0f, true);
	}

	AHeroBase* hero = Cast<AHeroBase>(PlayerPawn);

	float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (distance >= ChaseRange || (hero && hero->IsDead()))
	{
		// Return to spawn if lost player
		AIController->MoveToLocation(m_spawnLocation);

		if (this->GetActorLocation().Equals(m_spawnLocation))
		{
			// Returned home
			SetState(EBehaviourStates::IDLE);
			UE_LOG(LogZeroes, Log, TEXT("Player either out of range or dead."));
		}
	}
	else if (distance <= AttackMinDistance)
	{
		// Stop movement to attack
		AIController->StopMovement();

		SetState(EBehaviourStates::ATTACK);
		UE_LOG(LogZeroes, Log, TEXT("In range of player, attacking them"));
	}
}

void AEnemyBase::AttackStart()
{
	Event = GameEvents::ON_UPDATE;
	bCanPerformAttack = true;
}

void AEnemyBase::AttackUpdate()
{
	if (bCanPerformAttack)
	{		
		//AHeroState* state = Cast<AHeroState>(PlayerPawn->GetPlayerState());
		//if (state->GetHealth() <= 0)
		//{
		//	UE_LOG(LogZeroes, Log, TEXT("Player died while attacking. Idling..."));
		//	SetState(EBehaviourStates::IDLE);
		//	return;
		//}

		bCanPerformAttack = false;

		GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &AEnemyBase::OnAtkCooldownFinished, AtkCooldownDuration);
		UE_LOG(LogZeroes, Log, TEXT("Attacked Player. Cooling down"));

		if (PlayerPawn)
		{
			// Broadcast IsAttacking event
			if (OnEnemyBeginAttack.IsBound())
				OnEnemyBeginAttack.Broadcast();
		}
	}

	// Always look at player whilst waiting to attack
	FRotator rotation = UZeroesHelper::LookAtTarget(this->GetActorLocation(), PlayerPawn->GetActorLocation(), this->GetActorRotation());
	this->SetActorRotation(rotation);

	// Transition to chase once player is out of attack distance and enemy isn't attacking
	float distance = FVector::Distance(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (distance > AttackMinDistance && AnimInstance && !AnimInstance->bIsAttacking)
	{
		SetState(EBehaviourStates::CHASE);
		UE_LOG(LogZeroes, Log, TEXT("Player went out of range. Go bk to them"));
	}

	AHeroBase* hero = Cast<AHeroBase>(PlayerPawn);
	if (hero && hero->IsDead())
	{
		SetState(EBehaviourStates::CHASE);
		UE_LOG(LogZeroes, Log, TEXT("Player died while being attacked. Moving to original location"));
	}
}

void AEnemyBase::DeadStart()
{
	Event = GameEvents::ON_UPDATE;

	UE_LOG(LogZeroes, Log, TEXT("'%s' Death Entry state begin"), *this->GetName());

	DetachFromControllerPendingDestroy();

	// Configure cpasule component for physics
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SetActorEnableCollision(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	// Set time destroy after duration
	SetLifeSpan(10.0f);

	// Destroy UI for health
	WidgetComponent->DestroyComponent();
}

void AEnemyBase::DeadUpdate()
{
	
}

void AEnemyBase::OnAtkCooldownFinished()
{
	bCanPerformAttack = true;
	UE_LOG(LogZeroes, Log, TEXT("Cooldown finished. Can atk again"));
}