#pragma once

#include "S1P_AnimTypes.h" // Подключаем структуру FAnimStateData
#include "S1P_Types.h" // Подключаем перечисления ES1P_WeaponType и ES1P_MovementState

// ВАЖНОЕ ИЗМЕНЕНИЕ: Уберите BlueprintInternalUseOnly и добавьте правильные метаданные
USTRUCT(BlueprintType, meta = (DisplayName = "Anim State Data"))
struct SYSTEM1PARADOX_API FAnimStateData
{
    GENERATED_BODY()

public:
    FAnimStateData()
    {
        Speed = 0.0f;
        SmoothSpeed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = ES1P_WeaponType::Unarmed;
        MovementState = ES1P_MovementState::Idle;
        UnarmedAlpha = 1.0f;
        PistolAlpha = 0.0f;
        RifleAlpha = 0.0f;
        MeleeAlpha = 0.0f;
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
};