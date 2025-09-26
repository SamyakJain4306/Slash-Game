// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter/BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "SlashCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMotage;



UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	
	ASlashCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;


protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashInputContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ThrowAction;


	//Input Call backs
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
	void Throw();


	//Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;


	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DisarmMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* ArmMontage;

	void PlayDisarmMontage();
	void PlayArmMontage();



	UFUNCTION(BlueprintCallable)
	void ArmWeapon();

	UFUNCTION(BlueprintCallable)
	void DisarmWeapon();

	void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void ArmStart();

	UFUNCTION(BlueprintCallable)
	void ArmEnd();

	UFUNCTION(BlueprintCallable)
	void DisarmStart();

	UFUNCTION(BlueprintCallable)
	void DisarmEnd();

	UFUNCTION(BlueprintCallable)

	void HitReactEnd();






private:	
	AItem* OverLappedItem;


	EEquipStates CharacterEquipState = EEquipStates::EES_Unequipped;

	EAttackStates CharacterAttackState = EAttackStates::EAS_Unoccupied;


public:
	FORCEINLINE void SetOverlappedItem(AItem* Item) { OverLappedItem = Item; }
	FORCEINLINE EEquipStates GetEquipState() { return CharacterEquipState; }
};
