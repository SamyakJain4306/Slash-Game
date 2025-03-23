// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class AWeapon;




UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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

	//Input Call backs
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
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
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DisarmMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* ArmMontage;

	void PlayAttackMontage();
	void PlayDisarmMontage();
	void PlayArmMontage();

	UFUNCTION(BlueprintCallable)
	void ArmWeapon();

	UFUNCTION(BlueprintCallable)
	void DisarmWeapon();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void WeaponCollisionEnable();

	UFUNCTION(BlueprintCallable)
	void WeaponCollisionDisable();

	UFUNCTION(BlueprintCallable)
	void ArmStart();

	UFUNCTION(BlueprintCallable)
	void ArmEnd();

	UFUNCTION(BlueprintCallable)
	void DisarmStart();

	UFUNCTION(BlueprintCallable)
	void DisarmEnd();

private:	
	AItem* OverLappedItem;

	AWeapon* EquippedWeapon;

	EEquipStates CharacterEquipState = EEquipStates::EES_Unequipped;

	EAttackStates CharacterAttackState = EAttackStates::EAS_Unoccupied;


public:
	FORCEINLINE void SetOverlappedItem(AItem* Item) { OverLappedItem = Item; }
	FORCEINLINE EEquipStates GetEquipState() { return CharacterEquipState; }
};
