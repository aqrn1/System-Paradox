#include "CharacterAnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
    Speed = 0.f;
    bIsInAir = false;
    bIsCrouching = false;
    bIsAiming = false;
    bIsReloading = false;
    WeaponType = ES1P_WeaponType::Unarmed;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* Pawn = TryGetPawnOwner();
    if (!Pawn)
    {
        return;
    }

    OwningCharacter = Cast<ASystem1ParadoxCharacter>(Pawn);
}

// Пример синхронизации данных в AnimInstance
void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    if (ASystem1ParadoxCharacter* Character = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner()))
    {
        bIsCrouching = Character->bIsCrouching;
        bIsSprinting = Character->bIsSprinting;
        bIsInAir = Character->bIsInAir;
        Speed = Character->Speed;
        MovementDirection = Character->GetVelocity().GetSafeNormal();
        CurrentWeaponType = Character->CurrentWeaponType;
    }
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwningCharacter)
    {
        APawn* Pawn = TryGetPawnOwner();
        if (!Pawn)
        {
            return;
        }

        OwningCharacter = Cast<ASystem1ParadoxCharacter>(Pawn);
        if (!OwningCharacter)
        {
            return;
        }
    }

    UpdateFromCharacter();
}

void UCharacterAnimInstance::UpdateFromCharacter()
{
    if (!OwningCharacter)
    {
        return;
    }

    // ===== MOVEMENT =====
    FVector Velocity = OwningCharacter->GetVelocity();
    Velocity.Z = 0.f;

    Speed = Velocity.Size();

    UCharacterMovementComponent* MoveComp = OwningCharacter->GetCharacterMovement();
    if (MoveComp)
    {
        bIsInAir = MoveComp->IsFalling();
        bIsCrouching = MoveComp->IsCrouching();
    }

    // ===== WEAPON / ACTION =====
    WeaponType = OwningCharacter->GetCurrentWeaponType();
    bIsAiming = OwningCharacter->IsWeaponAiming();
    bIsReloading = OwningCharacter->IsWeaponReloading();
}
