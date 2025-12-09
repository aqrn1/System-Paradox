#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
// ВАЖНО: Подключаем заголовок, где объявлены нужные классы
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // ==================== PUBLIC VARIABLES FOR BLUEPRINT ====================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    ASystem1ParadoxCharacter::EWeaponType CurrentWeaponType;  // Исправлено: добавлен префикс класса

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    ASystem1ParadoxCharacter::EMovementState MovementState;   // Исправлено: добавлен префикс класса

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString WeaponName;

private:
    UPROPERTY()
    ASystem1ParadoxCharacter* CharacterPtr;

    void UpdateAnimationParameters();
    FString GetWeaponNameFromType(ASystem1ParadoxCharacter::EWeaponType WeaponType) const; // Исправлено
};