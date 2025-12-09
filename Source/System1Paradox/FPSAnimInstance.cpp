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
    bIsCrouching = PlayerCharacter->bIsCrouching;

    // Получаем состояние спринта
    bIsSprinting = PlayerCharacter->bIsSprinting;

    // Проверяем в воздухе ли персонаж
    bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

    // Получаем тип оружия
    CurrentWeaponType = PlayerCharacter->CurrentWeaponType;

    // Получаем состояние смены оружия
    bIsSwitchingWeapon = PlayerCharacter->bIsSwitchingWeapon;

    // Конвертируем тип оружия в строку для Blueprint
    switch (CurrentWeaponType)
    {
    case EWeaponType::Pistol:
        WeaponState = "Pistol";
        break;
    case EWeaponType::Rifle:
        WeaponState = "Rifle";
        break;
    default:
        WeaponState = "Unarmed";
        break;
    }

    // Определяем состояние движения
    if (bIsInAir)
    {
        MovementState = "Jumping";
    }
    else if (bIsCrouching)
    {
        MovementState = "Crouching";
    }
    else if (bIsSprinting && Speed > 100.0f)
    {
        MovementState = "Sprinting";
    }
    else if (Speed > 10.0f)
    {
        MovementState = "Walking";
    }
    else
    {
        MovementState = "Idle";
    }

    // Дебаг в консоль
    // GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, 
    //     FString::Printf(TEXT("Weapon: %s | State: %s | Speed: %.1f"), 
    //     *WeaponState, *MovementState, Speed));
}