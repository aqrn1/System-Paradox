#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "System1ParadoxCharacter.h"  // Включаем класс персонажа
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // Обязательная функция
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
    // Все переменные которые нужны для анимаций
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsSwitchingWeapon;

    // Состояния для Blueprint
    UPROPERTY(BlueprintReadOnly, Category = "States")
    FString WeaponState; // "Unarmed", "Pistol", "Rifle"

    UPROPERTY(BlueprintReadOnly, Category = "States")
    FString MovementState; // "Idle", "Walking", "Sprinting", "Crouching"

private:
    // Ссылка на персонажа
    UPROPERTY()
    class ASystem1ParadoxCharacter* PlayerCharacter;
};