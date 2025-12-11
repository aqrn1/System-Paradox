#pragma once

#include "CoreMinimal.h"
#include "S1P_Types.h"
#include "S1P_AnimTypes.generated.h"

// ВАЖНО: Эта строка ДОЛЖНА БЫТЬ ПЕРВОЙ после #include!
USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = "true"))
struct SYSTEM1PARADOX_API FAnimStateData
{
    GENERATED_BODY()

public:
    FAnimStateData()
    {
        Speed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = ES1P_WeaponType::Unarmed;
        MovementState = ES1P_MovementState::Idle;
        SmoothSpeed = 0.0f;
        UnarmedAlpha = 1.0f;
        PistolAlpha = 0.0f;
        RifleAlpha = 0.0f;
        MeleeAlpha = 0.0f;
    }

    // Основные данные движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    bool bIsCrouching;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    bool bIsSprinting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    bool bIsInAir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    ES1P_WeaponType CurrentWeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    ES1P_MovementState MovementState;

    // Для плавных переходов анимации
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float SmoothSpeed;

    // Веса для смешивания анимаций оружия
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float UnarmedAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float PistolAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float RifleAlpha;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
    float MeleeAlpha;
};