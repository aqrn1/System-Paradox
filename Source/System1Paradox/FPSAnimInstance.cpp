#include "FPSAnimInstance.h"
#include "System1ParadoxCharacter.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    Speed = 0.0f;
    bIsCrouching = false;
    bIsSprinting = false;
    bIsInAir = false;
    CurrentWeaponType = ES1P_WeaponType::Unarmed;
    MovementState = ES1P_MovementState::Idle;
    CharacterPtr = nullptr;
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    CharacterPtr = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (!CharacterPtr)
    {
        Speed = 0.0f;
        bIsCrouching = false;
        bIsSprinting = false;
        bIsInAir = false;
        CurrentWeaponType = ES1P_WeaponType::Unarmed;
        MovementState = ES1P_MovementState::Idle;
        return;
    }

    UpdateAnimationParameters();
}

void UFPSAnimInstance::UpdateAnimationParameters()
{
    Speed = CharacterPtr->GetCurrentSpeed();
    bIsCrouching = CharacterPtr->GetIsCrouching();
    bIsSprinting = CharacterPtr->GetIsSprinting();
    bIsInAir = CharacterPtr->GetIsInAir();
    CurrentWeaponType = CharacterPtr->GetCurrentWeaponType();
    MovementState = CharacterPtr->GetMovementState();
}