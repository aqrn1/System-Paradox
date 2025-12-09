#include "FPSAnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Получаем персонажа
    PlayerCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    // Если персонажа нет - выходим
    if (!PlayerCharacter) return;

    // Получаем скорость
    Speed = PlayerCharacter->GetVelocity().Size();

    // Получаем состояние приседания
    bIsCrouching = PlayerCharacter->GetIsCrouching();  // Стало
    bIsSprinting = PlayerCharacter->GetIsSprinting();  // Стало
    CurrentWeaponType = PlayerCharacter->GetCurrentWeaponType();  // Стало
    bIsSwitchingWeapon = PlayerCharacter->GetIsSwitchingWeapon();  // Стало

    // Конвертируем тип оружия в строку для Blueprint
    switch (CurrentWeaponType)
    {
    case EWeaponType::Pistol:
        WeaponState = TEXT("Pistol");
        break;
    case EWeaponType::Rifle:
        WeaponState = TEXT("Rifle");
        break;
    case EWeaponType::Melee:
        WeaponState = TEXT("Melee");
        break;
    default:
        WeaponState = TEXT("Unarmed");
        break;
    }

    // Определяем состояние движения
    if (bIsInAir)
    {
        MovementState = TEXT("Jumping");
    }
    else if (bIsCrouching)
    {
        MovementState = TEXT("Crouching");
    }
    else if (bIsSprinting && Speed > 100.0f)
    {
        MovementState = TEXT("Sprinting");
    }
    else if (Speed > 10.0f)
    {
        MovementState = TEXT("Walking");
    }
    else
    {
        MovementState = TEXT("Idle");
    }
}