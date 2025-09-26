// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"



UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Combat")
	AActor* CombatTarget;

protected:
	virtual void BeginPlay() override;





	//Variables
	UPROPERTY(VisibleInstanceOnly)
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere)
	class UAttributes* Attributes;

	

	//Warp Target Related Variables
	UPROPERTY(EditAnywhere, Category = "Combat")
	float WarpTargetDistance = 75.f;
	

	//Functions

	//Attack Related Functions
	int32 PlayAttackMontage();
	virtual void Attack();
	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void WeaponCollisionEnable();

	UFUNCTION(BlueprintCallable)
	void WeaponCollisionDisable();

	//Warp Target Related Functions
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget() const;

	//Damage Related Functions
	virtual void HandleDamage(float DamageAmount);
	void DirectionalHitMontage(const FVector& ImpactPoint);
	void PlayHitReactMontage(const FName& SectionName);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	//Death Related Functions
	virtual int32 PlayDeathMontage();
	virtual void Die();
	virtual bool IsAlive();



	

	


private:

	//AnimMontages
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Animation)
	TArray<FName> DeathMontageSections;

	//General Functions
	int32 PlayRandomMontage(UAnimMontage* Montage, TArray<FName> SectionNames);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	//Sounds
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSounds;

	//ParticleSystems
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

};
