#pragma once

#include "CoreMinimal.h"
#include "S1P_AnimTypes.generated.h"

UENUM(BlueprintType)
enum class ES1P_AnimMovementState : uint8
{
    Idle,
    Walk,
    Run,
    Sprint,
    Crouch
};

USTRUCT(BlueprintType)
struct FS1P_AnimState
{
    GENERATED_BODY()

    // ================== MOVEMENT ==================
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching = false;

    // ================== WEAPON ==================
    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsFiring = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsReloading = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsAiming = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    uint8 WeaponType = 0; // enum → uint8 (дёшево для сети)

    // ================== INTERNAL ==================
    UPROPERTY(BlueprintReadOnly, Category = "Internal")
    bool bIsValid = false;
};
