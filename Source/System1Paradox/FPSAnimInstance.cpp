#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    Speed = 0.0f;
    bIsCrouching = false;
    bIsSprinting = false;
    bIsInAir = false;
    CurrentWeaponType = EWeaponType::Unarmed;
    MovementState = TEXT("Idle");
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Получаем персонажа
    CharacterPtr = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (!CharacterPtr.IsValid())
    {
        Speed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = EWeaponType::Unarmed;
        MovementState = TEXT("Idle");
        return;
    }

    // Обновляем параметры через публичные геттеры
    Speed = CharacterPtr->GetCurrentSpeed();
    bIsCrouching = CharacterPtr->GetIsCrouching();
    bIsSprinting = CharacterPtr->GetIsSprinting();
    bIsInAir = CharacterPtr->GetIsInAir();
    CurrentWeaponType = CharacterPtr->GetCurrentWeaponType();

    // Определяем состояние движения
    UpdateMovementState();
}

void UFPSAnimInstance::UpdateMovementState()
{
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