#pragma once

#include "CoreMinimal.h"
#include "S1P_Types.h"  // Перечисления ES1P_WeaponType и ES1P_MovementState

#include "S1P_AnimTypes.generated.h"  // ← ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ!

USTRUCT(BlueprintType, meta = (DisplayName = "Anim State Data"))
struct SYSTEM1PARADOX_API FAnimStateData
{
    GENERATED_BODY()

public:
    FAnimStateData()
    {
        // Существующие поля
        Speed = 0.0f;
        SmoothSpeed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = ES1P_WeaponType::Unarmed;
        MovementState = ES1P_MovementState::Idle;

        // Оружие (уже есть)
        UnarmedAlpha = 1.0f;
        PistolAlpha = 0.0f;
        RifleAlpha = 0.0f;
        MeleeAlpha = 0.0f;

        // ★★★ ДОБАВЬТЕ ЭТИ ПОЛЯ ДЛЯ ОРУЖИЯ ★★★
        bIsFiring = false;
        bIsReloading = false;
        bIsAiming = false;
        FireAnimationTime = 0.0f;
        ReloadProgress = 0.0f;

        // ★★★ ДОПОЛНИТЕЛЬНЫЕ AAA ПОЛЯ ★★★
        AimPitch = 0.0f;           // Наклон прицеливания
        LeanAmount = 0.0f;         // Наклон тела вбок
        StrafeDirection = 0.0f;    // Направление страфа
        YawOffset = 0.0f;          // Смещение поворота тела
        bIsSliding = false;        // Скольжение
        bIsMantling = false;       // Перелезание
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SmoothSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
    bool bIsCrouching;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
    bool bIsSprinting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
    bool bIsInAir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    ES1P_WeaponType CurrentWeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    ES1P_MovementState MovementState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Weights")
    float UnarmedAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Weights")
    float PistolAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Weights")
    float RifleAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Weights")
    float MeleeAlpha;

    // ★★★ НОВЫЕ ПОЛЯ ДЛЯ ОРУЖИЯ ★★★
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsFiring;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsReloading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAiming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireAnimationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ReloadProgress;

    // ★★★ ДОПОЛНИТЕЛЬНЫЕ AAA ПОЛЯ ★★★
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    float AimPitch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    float LeanAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    float StrafeDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    float YawOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    bool bIsSliding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
    bool bIsMantling;
};