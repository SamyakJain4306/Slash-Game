// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Attributes.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "Items/Weapons/Weapon.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"


//Constructor
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("Enemy"));
	
	EnemyController = Cast<AAIController>(GetController());
	AiMoveToTarget(CurrentPetrolTarget);

	if(PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	}

	UWorld* World = GetWorld();	
	EquippedWeapon = World->SpawnActor<AWeapon>(Weapon);
	EquippedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"), this, this);
}


void AEnemy::Die()
{
	EnemyState = EEnemyStates::EES_Dead;
	PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
	EquippedWeapon->SetLifeSpan(5.f);
	ClearAttackTimer();
	

}
int32 AEnemy::PlayDeathMontage()
{
	int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_Max)
	{
		DeathPose = Pose;
	}

	return Selection;

}

void AEnemy::Attack()
{
	EnemyState = EEnemyStates::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::TargetInRange(AActor* Target, float Range)
{
	if (Target == nullptr) return false;
	const float distanceToTarget = (GetActorLocation() - Target->GetActorLocation()).Size();

	return distanceToTarget <= Range;
}

void AEnemy::PetrolTimerFinished()
{
	AiMoveToTarget(CurrentPetrolTarget);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	ClearAttackTimer();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CombatTarget = EventInstigator->GetPawn();
	
	HandleDamage(DamageAmount);
	
	ChaseCombatTarget();

	
	return DamageAmount;
}
void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes and HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		ShowHealthBar();
	}
}



void AEnemy::MoveToNewPetrolTarget()
{
	if (TargetInRange(CurrentPetrolTarget, 100.f))
	{
		CurrentPetrolTarget = ChoosePetrolTarget();
		GetWorldTimerManager().SetTimer(PetrolTimer, this, &AEnemy::PetrolTimerFinished, FMath::FRandRange(1.f, 5.f));
	}
}

bool AEnemy::CanAttack()
{
	return IsInsideAttackRadius() &&
		!IsEnemyAttacking() &&
		!IsEnemyEngaged() &&
		!IsDead();
}

bool AEnemy::ShouldChase(APawn* SelfPawn)
{
	bool bShouldChase =
		SelfPawn->ActorHasTag(FName("SlashCharacter")) &&
		EnemyState != EEnemyStates::EES_Dead &&
		EnemyState <= EEnemyStates::EES_Patrolling &&
		EnemyState != EEnemyStates::EES_Chasing;
	return bShouldChase;
}



// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsDead()) return;
	if(EnemyState > EEnemyStates::EES_Patrolling)
	{
		//Update Enemy State depending on the current distance to the combat target
		UpdateEnemyState();
	}
	else
	{
		//Move to new petrol target if current one is reached
		MoveToNewPetrolTarget();
	}
}

void AEnemy::UpdateEnemyState()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		if(!IsEnemyEngaged()) Petrol();
	}
	else if (IsOutsideAttackRadius() and !IsEnemyChasing())
	{
		ClearAttackTimer();
		if(!IsEnemyEngaged()) ChaseCombatTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

bool AEnemy::IsEnemyAttacking()
{
	return EnemyState == EEnemyStates::EES_Attacking;
}

bool AEnemy::IsInsideAttackRadius()
{
	return TargetInRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsEnemyChasing()
{
	return EnemyState == EEnemyStates::EES_Chasing;
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !TargetInRange(CombatTarget, AttackRadius);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyStates::EES_Attacking;
	float AttackDelay = FMath::FRandRange(1.f, 2.f);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackDelay);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyStates::EES_Dead;
}

bool AEnemy::IsEnemyEngaged()
{
	return EnemyState == EEnemyStates::EES_Engaged;
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !TargetInRange(CombatTarget, CombatRadius);
}

void AEnemy::ChaseCombatTarget()
{
	EnemyState = EEnemyStates::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	AiMoveToTarget(CombatTarget);
}

void AEnemy::Petrol()
{
	CombatTarget = nullptr;
	HideHealthBar();
	EnemyState = EEnemyStates::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PetrolSpeed;
	AiMoveToTarget(CurrentPetrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::AiMoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(5.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePetrolTarget()
{
	int32 Selection = FMath::RandRange(0, PetrolTargets.Num() - 1);
	if (PetrolTargets[Selection] == CurrentPetrolTarget)
	{
		return ChoosePetrolTarget();
	}
	return PetrolTargets[Selection];
}

void AEnemy::OnPawnSeen(APawn* Pawn)
{
	if (ShouldChase(Pawn))
	{
		CombatTarget = Pawn;
		ClearPetrolTimer();
		ChaseCombatTarget();
	}

}

void AEnemy::ClearPetrolTimer()
{
	GetWorldTimerManager().ClearTimer(PetrolTimer);
}
void AEnemy::AttackEnd()
{
	EnemyState = EEnemyStates::EES_NoState;
	UpdateEnemyState();
}


