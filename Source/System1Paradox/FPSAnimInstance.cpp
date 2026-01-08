#include "FPSAnimInstance.h"

// Game
#include "System1ParadoxCharacter.h"
#include "Weapon.h"

// UE
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    OwningCharacter = nullptr;
    bDebugMode = false;
    DebugSpeed = 0.0f;
    SmoothInterpSpeed = 8.0f;
}

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwningCharacter.IsValid())
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
    }

    UpdateAnimationState(DeltaSeconds);

    if (bDebugMode)
    {
        ApplyDebugValues();
    }
}

void UFPSAnimInstance::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();
    OwningCharacter = nullptr;
}

void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    if (!OwningCharacter.IsValid())
    {
        AnimState = FAnimStateData();
        return;
    }

    // ===== MOVEMENT =====
    const float Speed = OwningCharacter->GetVelocity().Size2D();

    AnimState.Speed = Speed;
    AnimState.SmoothSpeed = FMath::FInterpTo(
        AnimState.SmoothSpeed,
        Speed,
        DeltaSeconds,
        SmoothInterpSpeed
    );

    AnimState.bIsCrouching = OwningCharacter->GetIsCrouching();
    AnimState.bIsSprinting = OwningCharacter->GetIsSprinting();
    AnimState.bIsInAir = OwningCharacter->GetIsInAir();
    AnimState.CurrentWeaponType = OwningCharacter->GetCurrentWeaponType();

    // ===== MOVEMENT STATE =====
    if (AnimState.bIsInAir)
        AnimState.MovementState = ES1P_MovementState::Jumping;
    else if (AnimState.bIsCrouching)
        AnimState.MovementState = ES1P_MovementState::Crouching;
    else if (AnimState.bIsSprinting && Speed > 200.f)
        AnimState.MovementState = ES1P_MovementState::Sprinting;
    else if (Speed > 10.f)
        AnimState.MovementState = ES1P_MovementState::Walking;
    else
        AnimState.MovementState = ES1P_MovementState::Idle;

    // ===== WEAPON =====
    AWeapon* Weapon = OwningCharacter->GetCurrentWeapon();
    if (IsValid(Weapon))
    {
        AnimState.bIsFiring = Weapon->IsFiring();
        AnimState.bIsReloading = Weapon->IsReloading();
        AnimState.ReloadProgress = Weapon->GetReloadProgress();

        if (AnimState.bIsFiring)
        {
            AnimState.FireAnimationTime += DeltaSeconds;
            if (AnimState.FireAnimationTime > 0.25f)
                AnimState.FireAnimationTime = 0.f;
        }
        else
        {
            AnimState.FireAnimationTime = 0.f;
        }
    }
    else
    {
        AnimState.bIsFiring = false;
        AnimState.bIsReloading = false;
        AnimState.FireAnimationTime = 0.f;
        AnimState.ReloadProgress = 0.f;
    }

    UpdateWeaponBlendAlphas();

    // ===== AIM OFFSET / STRAFE =====
    const FRotator AimRot = OwningCharacter->GetControlRotation();
    const FRotator ActorRot = OwningCharacter->GetActorRotation();

    float YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(
        AimRot,
        ActorRot
    ).Yaw;

    AnimState.AimPitch = AimRot.Pitch;
    AnimState.YawOffset = FMath::FInterpTo(
        AnimState.YawOffset,
        YawDelta,
        DeltaSeconds,
        8.f
    );

    const FVector Velocity = OwningCharacter->GetVelocity();
    if (!Velocity.IsNearlyZero())
    {
        const FVector Forward = OwningCharacter->GetActorForwardVector();
        const FVector Right = OwningCharacter->GetActorRightVector();

        const float ForwardSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Forward);
        const float RightSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Right);

        AnimState.StrafeDirection = FMath::RadiansToDegrees(
            FMath::Atan2(RightSpeed, ForwardSpeed)
        );
    }
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    AnimState.UnarmedAlpha = 0.f;
    AnimState.PistolAlpha = 0.f;
    AnimState.RifleAlpha = 0.f;
    AnimState.MeleeAlpha = 0.f;

    switch (AnimState.CurrentWeaponType)
    {
    case ES1P_WeaponType::Unarmed: AnimState.UnarmedAlpha = 1.f; break;
    case ES1P_WeaponType::Pistol:  AnimState.PistolAlpha = 1.f; break;
    case ES1P_WeaponType::Rifle:   AnimState.RifleAlpha = 1.f; break;
    case ES1P_WeaponType::Melee:   AnimState.MeleeAlpha = 1.f; break;
    }
}

void UFPSAnimInstance::ApplyDebugValues()
{
    AnimState.Speed = DebugSpeed;
    AnimState.SmoothSpeed = DebugSpeed;
    AnimState.CurrentWeaponType = ES1P_WeaponType::Pistol;
    AnimState.MovementState = DebugSpeed > 10.f
        ? ES1P_MovementState::Walking
        : ES1P_MovementState::Idle;
}

void UFPSAnimInstance::AnimDebug(int32 Enable)
{
    bDebugMode = Enable != 0;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2.f,
            FColor::Yellow,
            bDebugMode ? TEXT("ANIM DEBUG ON") : TEXT("ANIM DEBUG OFF")
        );
    }
}

void UFPSAnimInstance::SetTestSpeed(float NewSpeed)
{
    DebugSpeed = FMath::Clamp(NewSpeed, 0.f, 1000.f);
    bDebugMode = true;
}
