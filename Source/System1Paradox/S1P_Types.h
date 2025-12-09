#pragma once

#include "CoreMinimal.h"
#include "S1P_Types.generated.h"

// ==================== цкнаюкэмше ENUM дкъ бяецн опнейрю ====================
UENUM(BlueprintType)
enum class ES1P_WeaponType : uint8
{
    Unarmed     UMETA(DisplayName = "Unarmed"),
    Pistol      UMETA(DisplayName = "Pistol"),
    Rifle       UMETA(DisplayName = "Rifle"),
    Melee       UMETA(DisplayName = "Melee")
};

UENUM(BlueprintType)
enum class ES1P_MovementState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Sprinting   UMETA(DisplayName = "Sprinting"),
    Crouching   UMETA(DisplayName = "Crouching"),
    Jumping     UMETA(DisplayName = "Jumping")
};