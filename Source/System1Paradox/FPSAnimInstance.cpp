#include "FPSAnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    CachedCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!CachedCharacter)
    {
        CachedCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
    }

    if (!CachedCharacter)
    {
        AnimState.bIsValid = false;
        return;
    }

    UpdateFromCharacter();
}

void UFPSAnimInstance::UpdateFromCharacter()
{
    UCharacterMovementComponent* MoveComp = CachedCharacter->GetCharacterMovement();
    if (!MoveComp)
    {
        AnimState.bIsValid = false;
        return;
    }

    AnimState.Speed = MoveComp->Velocity.Size();
    AnimState.bIsInAir = MoveComp->IsFalling();
    AnimState.bIsCrouching = MoveComp->IsCrouching();

    // Weapon state (ТОЛЬКО ЧТЕНИЕ)
    AnimState.bIsFiring = CachedCharacter->IsWeaponFiring();
    AnimState.bIsReloading = CachedCharacter->IsWeaponReloading();
    AnimState.bIsAiming = CachedCharacter->IsWeaponAiming();
    AnimState.WeaponType = (uint8)CachedCharacter->GetCurrentWeaponType();

    AnimState.bIsValid = true;
}
