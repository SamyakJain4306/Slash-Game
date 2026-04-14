// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterStates.h"
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(BlueprintReadOnly)
	EEnemyStates EnemyState = EEnemyStates::EES_Patrolling;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PetrolTargets;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* CurrentPetrolTarget;


	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PetrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PetrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> Weapon;

	

	UPROPERTY(EditAnywhere, Category = "Combat")
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDelayMin = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDelayMax = 3.f;

private:
	//Additional Components for the enemy charaacter
	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidget;
	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensingComponent;

	//Petrol Related Functions
	void Petrol();
	AActor* ChoosePetrolTarget();
	void MoveToNewPetrolTarget();
	void ClearPetrolTimer();
	void PetrolTimerFinished();

	//Chase Related Functions
	void ChaseCombatTarget();
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);// Called when the pawn sensing component detects a pawn(CallBack)

	//Attack Related Functions
	virtual void Attack() override;
	virtual void AttackEnd() override;
	void StartAttackTimer();
	void ClearAttackTimer();

	//Damage And Death Related Functions
	virtual void HandleDamage(float DamageAmount) override;
	void Die() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;																																																	


	//Checks
	virtual bool CanAttack() override;
	bool ShouldChase(APawn* SelfPawn);
	
	bool IsDead();
	bool IsEnemyEngaged();
	bool IsOutsideCombatRadius();
	bool IsEnemyAttacking();
	bool IsInsideAttackRadius();
	bool IsEnemyChasing();
	bool IsOutsideAttackRadius();


	//General Functions
	void UpdateEnemyState();
	void HideHealthBar();
	void ShowHealthBar();
	void AiMoveToTarget(AActor* Target);
	bool TargetInRange(AActor* Target, float Range);


	//Combat and Attack Related Variables
	

	float CombatRadius = 500.f;
	float AttackRadius = 200.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PetrolSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;

	//Petrol Related Variables
	FTimerHandle PetrolTimer;

};
