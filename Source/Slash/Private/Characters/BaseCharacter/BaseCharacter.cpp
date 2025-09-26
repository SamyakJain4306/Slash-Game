// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter/BaseCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "Components/Attributes.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::WeaponCollisionEnable()
{
	EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseCharacter::WeaponCollisionDisable()
{
	EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->IgnoreActors.Empty();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->RecieveDamage(DamageAmount);
	}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontage(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayRandomMontage(UAnimMontage* Montage, TArray<FName> SectionNames)
{
	int32 Selection = FMath::RandRange(0, SectionNames.Num() - 1);
	PlayAnimMontage(Montage, 1.f, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontage(DeathMontage, DeathMontageSections);
}
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

FVector ABaseCharacter::GetRotationWarpTarget() const
{ 
	if (IsValid(CombatTarget))
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

FVector ABaseCharacter::GetTranslationWarpTarget() const 
{
	if (!IsValid(CombatTarget)) 
	{
		
		return FVector();
	}
	const FVector TargetLocation = CombatTarget->GetActorLocation();
	const FVector MyLocation = GetActorLocation();
	FVector TargetToMe = (MyLocation - TargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	DrawDebugSphere(GetWorld(), TargetLocation + TargetToMe, 2.f, 12, FColor::Black);
	return TargetLocation + TargetToMe;
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() and Hitter)
	{
		DirectionalHitMontage(Hitter->GetActorLocation());
	}
	else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}
void ABaseCharacter::Die()
{

}
void ABaseCharacter::Attack()
{

}

void ABaseCharacter::DirectionalHitMontage(const FVector& ImpactPoint)
{
	FVector Forward = GetActorForwardVector();
	//ImpactLowered
	FVector ImpactLowered = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();


	double Theta = FMath::RadiansToDegrees(FMath::Acos((FVector::DotProduct(Forward, ToHit))));

	FVector Cross = FVector::CrossProduct(Forward, ToHit);

	if (Cross.Z < 0)
	{
		Theta *= -1.f;
	}

	FName SectionName = "FromBack";
	if (Theta <= -45 and Theta > -135)
	{
		SectionName = "FromLeft";
	}
	else if (Theta <= 45 and Theta > -45)
	{
		SectionName = "FromFront";
	}
	else if (Theta <= 135 and Theta > 45)
	{
		SectionName = "FromRight";
	}
	PlayHitReactMontage(SectionName);
}
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	PlayMontageSection(HitReactMontage, SectionName);
	UE_LOG(LogTemp, Warning, TEXT("Play Hit Montage function"))
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSounds)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSounds, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles and GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{

}
