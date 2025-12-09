#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    // Инициализация значений по умолчанию
    Speed = 0.0f;
    bIsCrouching = false;
    bIsSprinting = false;
    bIsInAir = false;
    CurrentWeaponType = ASystem1ParadoxCharacter::EWeaponType::Unarmed; // Исправлено
    WeaponName = TEXT("Unarmed");
    MovementState = ASystem1ParadoxCharacter::EMovementState::Idle; // Исправлено
    CharacterPtr = nullptr;
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Получаем ссылку на персонажа
    CharacterPtr = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    // Если персонаж не найден, сбрасываем значения
    if (!CharacterPtr)
    {
        Speed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = ASystem1ParadoxCharacter::EWeaponType::Unarmed; // Исправлено
        WeaponName = TEXT("Unarmed");
        MovementState = ASystem1ParadoxCharacter::EMovementState::Idle; // Исправлено
        return;
    }

    // Обновляем все параметры через публичные геттеры
    UpdateAnimationParameters();
}

void UFPSAnimInstance::UpdateAnimationParameters()
{
    // Безопасное получение значений
    Speed = CharacterPtr->GetCurrentSpeed();
    bIsCrouching = CharacterPtr->GetIsCrouching();
    bIsSprinting = CharacterPtr->GetIsSprinting();
    bIsInAir = CharacterPtr->GetIsInAir();
    CurrentWeaponType = CharacterPtr->GetCurrentWeaponType();
    MovementState = CharacterPtr->GetMovementState();

    // Конвертируем тип оружия в строку
    WeaponName = GetWeaponNameFromType(CurrentWeaponType);
}

FString UFPSAnimInstance::GetWeaponNameFromType(ASystem1ParadoxCharacter::EWeaponType WeaponType) const // Исправлено
{
    switch (WeaponType)
    {
    case ASystem1ParadoxCharacter::EWeaponType::Pistol: // Исправлено
        return TEXT("Pistol");
    case ASystem1ParadoxCharacter::EWeaponType::Rifle: // Исправлено
        return TEXT("Rifle");
    case ASystem1ParadoxCharacter::EWeaponType::Melee: // Исправлено
        return TEXT("Melee");
    case ASystem1ParadoxCharacter::EWeaponType::Unarmed: // Исправлено
    default:
        return TEXT("Unarmed");
    }
}