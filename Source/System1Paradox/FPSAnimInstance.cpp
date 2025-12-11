#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    OwningCharacter = nullptr;
    bDebugMode = false;
    DebugSpeed = 0.0f;
    SmoothInterpSpeed = 8.0f;
    bForceAnimStateUpdate = true;
}

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (GEngine)
    {
        if (OwningCharacter)
        {
            FString DebugMsg = FString::Printf(TEXT("ANIM INIT: Character %s"),
                *OwningCharacter->GetName());
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugMsg);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                TEXT("ANIM INIT: No Character Found"));
        }
    }
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // ВСЕГДА обновляем ссылку на персонажа
    if (!OwningCharacter)
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
        if (!OwningCharacter)
        {
            // Если персонажа нет, сбрасываем данные
            AnimState.Speed = 0.0f;
            AnimState.SmoothSpeed = 0.0f;
            return;
        }
    }

    // ВАЖНО: ВСЕГДА обновляем состояние!
    UpdateAnimationState(DeltaSeconds);

    // Режим отладки (перезаписывает значения)
    if (bDebugMode && DebugSpeed >= 0.0f)
    {
        AnimState.Speed = DebugSpeed;
        AnimState.SmoothSpeed = DebugSpeed;
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = (DebugSpeed > 400.0f);
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Pistol;
        AnimState.MovementState = (DebugSpeed > 10.0f) ?
            ES1P_MovementState::Walking : ES1P_MovementState::Idle;

        if (GEngine)
        {
            FString DebugMsg = FString::Printf(TEXT("DEBUG SPEED: %.0f"), DebugSpeed);
            GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, DebugMsg);
        }
    }

    UpdateWeaponBlendAlphas();

    if (bDebugMode && GEngine)
    {
        FString StateStr;
        switch (AnimState.MovementState)
        {
        case ES1P_MovementState::Idle: StateStr = "Idle"; break;
        case ES1P_MovementState::Walking: StateStr = "Walking"; break;
        case ES1P_MovementState::Sprinting: StateStr = "Sprinting"; break;
        case ES1P_MovementState::Crouching: StateStr = "Crouching"; break;
        case ES1P_MovementState::Jumping: StateStr = "Jumping"; break;
        default: StateStr = "Unknown"; break;
        }

        FString DebugText = FString::Printf(
            TEXT("ANIM: Speed=%.0f | State=%s | Crouch=%s | Sprint=%s | Air=%s"),
            AnimState.Speed,
            *StateStr,
            AnimState.bIsCrouching ? TEXT("Yes") : TEXT("No"),
            AnimState.bIsSprinting ? TEXT("Yes") : TEXT("No"),
            AnimState.bIsInAir ? TEXT("Yes") : TEXT("No")
        );

        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, DebugText);
    }

    if (bForceAnimStateUpdate)
    {
        bForceAnimStateUpdate = false;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
                TEXT("AnimState активно обновляется"));
        }
    }
}

void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    if (!OwningCharacter)
    {
        AnimState.Speed = 0.0f;
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = false;
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Unarmed;
        AnimState.MovementState = ES1P_MovementState::Idle;
        return;
    }

    AnimState.Speed = OwningCharacter->GetCurrentSpeed();
    AnimState.bIsCrouching = OwningCharacter->GetIsCrouching();
    AnimState.bIsSprinting = OwningCharacter->GetIsSprinting();
    AnimState.bIsInAir = OwningCharacter->GetIsInAir();
    AnimState.CurrentWeaponType = OwningCharacter->GetCurrentWeaponType();
    AnimState.MovementState = OwningCharacter->GetMovementState();

    ApplySmoothing(DeltaSeconds);
}

void UFPSAnimInstance::ApplySmoothing(float DeltaSeconds)
{
    if (DeltaSeconds > 0.0f)
    {
        AnimState.SmoothSpeed = FMath::FInterpTo(
            AnimState.SmoothSpeed,
            AnimState.Speed,
            DeltaSeconds,
            SmoothInterpSpeed
        );

        AnimState.SmoothSpeed = FMath::Clamp(AnimState.SmoothSpeed, 0.0f, 600.0f);
    }
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    switch (AnimState.CurrentWeaponType)
    {
    case ES1P_WeaponType::Unarmed:
        AnimState.UnarmedAlpha = 1.0f;
        break;
    case ES1P_WeaponType::Pistol:
        AnimState.PistolAlpha = 1.0f;
        break;
    case ES1P_WeaponType::Rifle:
        AnimState.RifleAlpha = 1.0f;
        break;
    case ES1P_WeaponType::Melee:
        AnimState.MeleeAlpha = 1.0f;
        break;
    default:
        AnimState.UnarmedAlpha = 1.0f;
        break;
    }
}

void UFPSAnimInstance::AnimDebug(int32 Enable)
{
    bDebugMode = (Enable != 0);

    // ПРИНУДИТЕЛЬНЫЙ ВЫВОД - всегда работает
    if (GEngine)
    {
        FString Message = bDebugMode ?
            TEXT("✅ ANIM DEBUG: ON (SetTestSpeed <value>)") :
            TEXT("✅ ANIM DEBUG: OFF");

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, Message);

        // Дублируем в лог
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }
    else
    {
        // Если GEngine недоступен
        UE_LOG(LogTemp, Error, TEXT("GEngine is NULL!"));
    }
}

void UFPSAnimInstance::SetTestSpeed(float NewSpeed)
{
    DebugSpeed = FMath::Clamp(NewSpeed, 0.0f, 1000.0f);
    bDebugMode = true;

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("TEST SPEED: %.0f"), DebugSpeed);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugMsg);
    }
}

void UFPSAnimInstance::TestAnimation(FName AnimationName)
{
    ForcedAnimation = AnimationName.ToString();
    bDebugMode = true;

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("FORCED ANIM: %s"), *ForcedAnimation);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, DebugMsg);
    }
}

