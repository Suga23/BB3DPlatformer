#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EIS_Melee UMETA(DisplayName = "Melee"),
	EIS_Range UMETA(DisplayName = "Range"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};