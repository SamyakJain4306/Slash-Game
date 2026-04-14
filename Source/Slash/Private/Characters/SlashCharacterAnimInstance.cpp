// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacterAnimInstance.h"
#include "Slash/Public/Characters/SlashCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void USlashCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());

	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
	
}

void USlashCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
	}
	if (SlashCharacter)
	{
		CharacterEquipStates = SlashCharacter->GetEquipState();
		CharacterActionState = SlashCharacter->GetCharacterActionState();
		DeathPose = SlashCharacter->GetDeathPose();
	}
	
	
}
