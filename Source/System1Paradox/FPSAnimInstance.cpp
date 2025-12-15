// FPSAnimInstance.cpp - ПОЛНОСТЬЮ ПЕРЕПИСАННЫЙ
#include "FPSAnimInstance.h"
#include "System1ParadoxCharacter.h"
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
        if (OwningCharacter.IsValid() && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                TEXT("✅ CHARACTER FOUND"));
        }
    }

    UpdateAnimationState(DeltaSeconds);
    UpdateWeaponBlendAlphas();

    if (bDebugMode) ApplyDebugValues();
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
        AnimState.Speed = 0.0f;
        AnimState.SmoothSpeed = 0.0f;
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = false;
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Unarmed;
        AnimState.MovementState = ES1P_MovementState::Idle;
        return;
    }

    float Speed = OwningCharacter->GetVelocity().Size2D();
    ES1P_WeaponType WeaponType = OwningCharacter->GetCurrentWeaponType();
    bool bIsCrouching = OwningCharacter->GetIsCrouching();
    bool bIsSprinting = OwningCharacter->GetIsSprinting();
    bool bIsInAir = OwningCharacter->GetIsInAir();

    AnimState.Speed = Speed;
    AnimState.SmoothSpeed = FMath::FInterpTo(AnimState.SmoothSpeed, Speed, DeltaSeconds, SmoothInterpSpeed);
    AnimState.bIsCrouching = bIsCrouching;
    AnimState.bIsSprinting = bIsSprinting;
    AnimState.bIsInAir = bIsInAir;
    AnimState.CurrentWeaponType = WeaponType;

    if (AnimState.bIsInAir)
        AnimState.MovementState = ES1P_MovementState::Jumping;
    else if (AnimState.bIsCrouching)
        AnimState.MovementState = ES1P_MovementState::Crouching;
    else if (AnimState.bIsSprinting && Speed > 100.0f)
        AnimState.MovementState = ES1P_MovementState::Sprinting;
    else if (Speed > 10.0f)
        AnimState.MovementState = ES1P_MovementState::Walking;
    else
        AnimState.MovementState = ES1P_MovementState::Idle;
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    switch (AnimState.CurrentWeaponType)
    {
    case ES1P_WeaponType::Unarmed: AnimState.UnarmedAlpha = 1.0f; break;
    case ES1P_WeaponType::Pistol: AnimState.PistolAlpha = 1.0f; break;
    case ES1P_WeaponType::Rifle: AnimState.RifleAlpha = 1.0f; break;
    case ES1P_WeaponType::Melee: AnimState.MeleeAlpha = 1.0f; break;
    }
}

void UFPSAnimInstance::ApplyDebugValues()
{
    if (DebugSpeed >= 0.0f)
    {
        AnimState.Speed = DebugSpeed;
        AnimState.SmoothSpeed = DebugSpeed;
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = (DebugSpeed > 400.0f);
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Pistol;
        AnimState.MovementState = (DebugSpeed > 10.0f) ?
            ES1P_MovementState::Walking : ES1P_MovementState::Idle;
    }
}

void UFPSAnimInstance::AnimDebug(int32 Enable)
{
    bDebugMode = (Enable != 0);
    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("🔧 ANIM DEBUG: %s"),
            bDebugMode ? TEXT("ON") : TEXT("OFF"));
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMsg);
    }
}

void UFPSAnimInstance::SetTestSpeed(float NewSpeed)
{
    DebugSpeed = FMath::Clamp(NewSpeed, 0.0f, 1000.0f);
    bDebugMode = true;

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("🚀 TEST SPEED: %.0f"), DebugSpeed);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugMsg);
    }
}