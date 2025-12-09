#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

// Состояния оружия для Blueprint
UENUM(BlueprintType)
enum class EFPSWeaponState : uint8
{
    Unarmed     UMETA(DisplayName = "Unarmed"),
    Pistol      UMETA(DisplayName = "Pistol"),
    Rifle       UMETA(DisplayName = "Rifle"),
    Melee       UMETA(DisplayName = "Melee")
};

// Состояния движения для Blueprint
UENUM(BlueprintType)
enum class EFPSMovementState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Sprinting   UMETA(DisplayName = "Sprinting"),
    Crouching   UMETA(DisplayName = "Crouching"),
    Jumping     UMETA(DisplayName = "Jumping")
};

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    // Основная функция обновления анимаций
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // ==================== ПУБЛИЧНЫЕ ПЕРЕМЕННЫЕ ДЛЯ BLUEPRINT ====================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsInAir;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EFPSWeaponState WeaponState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    bool bIsSwitchingWeapon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    EFPSMovementState MovementState;

protected:
    // Ссылка на персонажа
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<class ASystem1ParadoxCharacter> CharacterPtr;

private:
    // Вспомогательные функции
    void UpdateMovementState();
    void UpdateWeaponState();
    EFPSWeaponType ConvertWeaponType(EWeaponType WeaponType) const;
};