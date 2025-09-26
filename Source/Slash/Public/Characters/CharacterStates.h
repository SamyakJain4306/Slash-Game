#pragma once
UENUM(BlueprintType)
enum class EEquipStates: uint8
{
	EES_Unequipped UMETA(DisplayName = "Unequipped"),
	EES_Equipped UMETA(DisplayName = "Equipped Holding One-Handed"),
	EES_Equipping UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum class EAttackStates: uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReacting UMETA(DisplayName = "Hit Reacting"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),

	EDP_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	EES_NoState UMETA(DisplayName = "No State"),

	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")


};