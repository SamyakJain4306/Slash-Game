// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"
#include "Animation/AnimInstance.h"
#include "Components/Attributes.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"


ASlashCharacter::ASlashCharacter()
{

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
 	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);


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

	Tags.Add(FName("SlashCharacter"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashInputContext, 0);
		}
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
		}
	}
	if (SlashOverlay)
	{
		SlashOverlay->SetHealthPercent(100.f);
		SlashOverlay->SetStaminaPercent(100.f);
		SlashOverlay->SetGoldAmount(0);
		SlashOverlay->SetSoulsAmount(0);
	}

}
void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (CharacterActionState == EActionStates::EAS_Attacking or CharacterEquipState == EEquipStates::EES_Equipping)
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
		
		if (OverlappedWeapon)
		{
			OverlappedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"), this, this);
			CharacterEquipState = EEquipStates::EES_Equipped;
		}
		EquippedWeapon = OverlappedWeapon;
		OverLappedItem = nullptr;
	}
	else if (CharacterEquipState == EEquipStates::EES_Equipped and CharacterActionState == EActionStates::EAS_Unoccupied and EquippedWeapon)
	{
		PlayDisarmMontage();
	}
	else if (CharacterEquipState == EEquipStates::EES_Unequipped and CharacterActionState == EActionStates::EAS_Unoccupied and EquippedWeapon)
	{
		PlayArmMontage();
	}
}

void ASlashCharacter::ArmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"), this, this);
	}
}

void ASlashCharacter::DisarmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeapon(GetMesh(), FName("SwordHolderSocket"), this, this);
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
	}
}

void ASlashCharacter::Throw()
{
	if (EquippedWeapon == nullptr) return;
	EquippedWeapon->DettachWeapon();
	CharacterEquipState = EEquipStates::EES_Unequipped;
	EquippedWeapon = nullptr;

}

void ASlashCharacter::Die()
{
	Super::Die();
	CharacterActionState = EActionStates::EAS_Dead;
	GetCharacterMovement()->DisableMovement();
}

void ASlashCharacter::Jump()
{
	if (CharacterActionState  == EActionStates::EAS_Unoccupied)
	{
		Super::Jump();
	}
}


void ASlashCharacter::Attack()
{
	if (CharacterActionState == EActionStates::EAS_Unoccupied and CharacterEquipState != EEquipStates::EES_Unequipped and CharacterEquipState != EEquipStates::EES_Equipping)
	{
		PlayAttackMontage();
		CharacterActionState = EActionStates::EAS_Attacking;
	}
	
}



void ASlashCharacter::AttackEnd()
{
	CharacterActionState = EActionStates::EAS_Unoccupied;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (IsAlive())
	{
		CharacterActionState = EActionStates::EAS_HitReacting;
		if (EquippedWeapon) WeaponCollisionDisable();
		CombatTarget = Hitter;
	}

}


void ASlashCharacter::ArmStart()
{
	CharacterEquipState = EEquipStates::EES_Equipping;
}

void ASlashCharacter::ArmEnd()
{
	CharacterEquipState = EEquipStates::EES_Equipped;
}

void ASlashCharacter::DisarmStart()
{
	CharacterEquipState = EEquipStates::EES_Equipping;
}

void ASlashCharacter::DisarmEnd()
{
	CharacterEquipState = EEquipStates::EES_Unequipped;
}

void ASlashCharacter::HitReactEnd()
{
	CharacterActionState = EActionStates::EAS_Unoccupied;
}

void ASlashCharacter::SetOverlappedItem(AItem* OverlappingItem)
{
	OverLappedItem = OverlappingItem;
}

void ASlashCharacter::AddSoul(ASoul* Soul)
{
	UE_LOG(LogTemp, Warning, TEXT("ASlashCharacter::AddSoul"));
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
		EnhancedInputCompontent->BindAction(ThrowAction, ETriggerEvent::Started, this, &ASlashCharacter::Throw);

	}

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	if (SlashOverlay and Attributes)
	{
		SlashOverlay->SetHealthPercent(Attributes->GetHealthPercent());
	}
	return DamageAmount;
}

