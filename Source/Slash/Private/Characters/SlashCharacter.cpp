// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"


ASlashCharacter::ASlashCharacter()
{

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
 	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

}


void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashInputContext, 0);
		}
	}

}
void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (CharacterAttackState == EAttackStates::EAS_Attacking or CharacterEquipState == EEquipStates::EES_Equipping)
	{
		return;
	}
	const FVector2d Value2D =  Value.Get<FVector2d>();
	if (Controller)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, Value2D.Y);
		AddMovementInput(RightDirection, Value2D.X);
	}
}
void ASlashCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(-Value.Get<FVector2D>().Y);
	AddControllerYawInput(Value.Get<FVector2D>().X);
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappedWeapon = Cast<AWeapon>(OverLappedItem);
	if (CharacterEquipState == EEquipStates::EES_Unequipped and OverlappedWeapon)
	{
		OverlappedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"));
		CharacterEquipState = EEquipStates::EES_EquippedHoldingOneHand;
		EquippedWeapon = OverlappedWeapon;
		OverLappedItem = nullptr;
		
	}
	else if (CharacterEquipState == EEquipStates::EES_EquippedHoldingOneHand and CharacterAttackState == EAttackStates::EAS_Unoccupied)
	{
		PlayDisarmMontage();
	}
	else if (CharacterEquipState == EEquipStates::EES_Unequipped and CharacterAttackState == EAttackStates::EAS_Unoccupied and EquippedWeapon)
	{
		PlayArmMontage();
	}
}

void ASlashCharacter::ArmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::DisarmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeapon(GetMesh(), FName("SwordHolderSocket"));
	}
}

void ASlashCharacter::PlayDisarmMontage()
{
	USkeletalMeshComponent* SKM = GetMesh();
	UAnimInstance* AnimInstance = SKM->GetAnimInstance();
	if (AnimInstance and DisarmMontage)
	{
		AnimInstance->Montage_Play(DisarmMontage);
	}
}

void ASlashCharacter::PlayArmMontage()
{
	USkeletalMeshComponent* SKM = GetMesh();
	UAnimInstance* AnimInstance = SKM->GetAnimInstance();
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, "Montage");
	if (AnimInstance and ArmMontage)
	{
		AnimInstance->Montage_Play(ArmMontage);
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, "PlayedMontage");
	}
}



void ASlashCharacter::Attack()
{
	if (CharacterAttackState == EAttackStates::EAS_Unoccupied and CharacterEquipState == EEquipStates::EES_EquippedHoldingOneHand)
	{
		PlayAttackMontage();
		CharacterAttackState = EAttackStates::EAS_Attacking;
	}
	
}

void ASlashCharacter::PlayAttackMontage()
{
	USkeletalMeshComponent* SKM = GetMesh();
	UAnimInstance* AnimInstance = SKM->GetAnimInstance();
	if (AnimInstance and AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName;
		int32 Selection = FMath::RandRange(1, 2);
		switch (Selection)
		{
		case 1:
			SectionName = FName("Attack1");
			break;
		case 2:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	CharacterAttackState = EAttackStates::EAS_Unoccupied;
}

void ASlashCharacter::WeaponCollisionEnable()
{
	EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASlashCharacter::WeaponCollisionDisable()
{
	EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->IgnoreActors.Empty();
}

void ASlashCharacter::ArmStart()
{
	CharacterEquipState = EEquipStates::EES_Equipping;
}

void ASlashCharacter::ArmEnd()
{
	CharacterEquipState = EEquipStates::EES_EquippedHoldingOneHand;
}

void ASlashCharacter::DisarmStart()
{
	CharacterEquipState = EEquipStates::EES_Equipping;
}

void ASlashCharacter::DisarmEnd()
{
	CharacterEquipState = EEquipStates::EES_Unequipped;
}



void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputCompontent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputCompontent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputCompontent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputCompontent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputCompontent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputCompontent->BindAction(EquipAction, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputCompontent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);

	}

}




void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}