#pragma once
UENUM(BlueprintType)
enum class EEquipStates: uint8
{
	EES_Unequipped UMETA(DisplayName = "Unequipped"),
	EES_EquippedHoldingOneHand UMETA(DisplayName = "Equipped Holding One-Handed"),
	EES_EquippedHoldingTwoHands UMETA(DisplayName = "Equipped Holding Two-Handed"),
	EES_Equipping UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum class EAttackStates: uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};