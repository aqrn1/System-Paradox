#pragma once

#include "CoreMinimal.h"
#include "S1P_Types.h"  // Включаем для ES1P_WeaponType и ES1P_MovementState
#include "S1P_AnimTypes.generated.h"

/**
 * Структура для передачи данных анимации из C++ в Blueprint
 * Используется как "Cached Anim State Data" в Animation Blueprint
 */
USTRUCT(BlueprintType)
struct SYSTEM1PARADOX_API FAnimStateData
{
    GENERATED_BODY()

public:
    FAnimStateData()
        : Speed(0.0f)
        , bIsCrouching(false)
        , bIsSprinting(false)
        , bIsInAir(false)
        , CurrentWeaponType(ES1P_WeaponType::Unarmed)
        , MovementState(ES1P_MovementState::Idle)
        , SmoothSpeed(0.0f)
        , UnarmedAlpha(1.0f)
        , PistolAlpha(0.0f)
        , RifleAlpha(0.0f)
        , MeleeAlpha(0.0f)
    {
    }

    /** Текущая скорость персонажа (см/с) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float Speed;

    /** Приседает ли персонаж */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    bool bIsCrouching;

    /** Бежит ли персонаж */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    bool bIsSprinting;

    /** Находится ли персонаж в воздухе */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    bool bIsInAir;

    /** Текущий тип оружия */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    ES1P_WeaponType CurrentWeaponType;

    /** Текущее состояние движения */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    ES1P_MovementState MovementState;

    /** Сглаженная скорость для плавных переходов анимации */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float SmoothSpeed;

    /** Вес для анимаций без оружия (0-1) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float UnarmedAlpha;

    /** Вес для анимаций пистолета (0-1) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float PistolAlpha;

    /** Вес для анимаций винтовки (0-1) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float RifleAlpha;

    /** Вес для анимаций холодного оружия (0-1) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    float MeleeAlpha;
};