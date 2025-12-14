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

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // ОТЛАДКА: Всегда показываем состояние
    static int32 FrameCounter = 0;
    FrameCounter++;

    if (GEngine && (FrameCounter % 30 == 0)) // Каждые 0.5 секунды
    {
        APawn* Pawn = TryGetPawnOwner();
        FString PawnName = Pawn ? *Pawn->GetName() : TEXT("NULL");

        FString DebugMsg = FString::Printf(
            TEXT("🔍 ANIM DEBUG | Pawn: %s | Char: %s | Frame: %d"),
            *PawnName,
            OwningCharacter ? TEXT("VALID") : TEXT("NULL"),
            FrameCounter
        );
        GEngine->AddOnScreenDebugMessage(200, 1.0f, FColor::Orange, DebugMsg);
    }

    // 1. ПОЛУЧАЕМ ССЫЛКУ НА ПЕРСОНАЖА
    if (!OwningCharacter)
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

        if (OwningCharacter && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("✅ CHARACTER FOUND: %s"), *OwningCharacter->GetName()));
        }
        else if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
                TEXT("❌ NO CHARACTER"));
        }
    }

    // 2. ЕСЛИ ПЕРСОНАЖ ЕСТЬ - ОБНОВЛЯЕМ ДАННЫЕ
    if (OwningCharacter)
    {
        // ПРИНУДИТЕЛЬНАЯ ПРОВЕРКА ЗНАЧЕНИЙ
        float Speed = OwningCharacter->GetVelocity().Size2D(); // Прямой доступ
        ES1P_WeaponType WeaponType = OwningCharacter->GetCurrentWeaponType();
        bool bIsCrouching = OwningCharacter->bIsCrouched; // Прямой доступ к переменной ACharacter

        if (GEngine && (FrameCounter % 60 == 0))
        {
            FString ValueMsg = FString::Printf(
                TEXT("📊 VALUES | Speed: %.0f | Weapon: %d | Crouch: %s"),
                Speed,
                (int32)WeaponType,
                bIsCrouching ? TEXT("Yes") : TEXT("No")
            );
            GEngine->AddOnScreenDebugMessage(201, 1.0f, FColor::Yellow, ValueMsg);
        }

        // ВАЖНО: ЗАПОЛНЯЕМ СТРУКТУРУ ПРЯМО ЗДЕСЬ
        AnimState.Speed = Speed;
        AnimState.SmoothSpeed = FMath::FInterpTo(AnimState.SmoothSpeed, Speed, DeltaSeconds, SmoothInterpSpeed);
        AnimState.bIsCrouching = bIsCrouching;
        AnimState.bIsSprinting = OwningCharacter->GetIsSprinting();
        AnimState.bIsInAir = OwningCharacter->GetCharacterMovement()->IsFalling();
        AnimState.CurrentWeaponType = WeaponType;

        // Определяем состояние движения
        if (AnimState.bIsInAir) {
            AnimState.MovementState = ES1P_MovementState::Jumping;
        }
        else if (AnimState.bIsCrouching) {
            AnimState.MovementState = ES1P_MovementState::Crouching;
        }
        else if (AnimState.bIsSprinting && Speed > 100.0f) {
            AnimState.MovementState = ES1P_MovementState::Sprinting;
        }
        else if (Speed > 10.0f) {
            AnimState.MovementState = ES1P_MovementState::Walking;
        }
        else {
            AnimState.MovementState = ES1P_MovementState::Idle;
        }

        UpdateWeaponBlendAlphas();

        // ОТЛАДКА: Показываем финальные значения
        if (GEngine && (FrameCounter % 90 == 0))
        {
            FString FinalMsg = FString::Printf(
                TEXT("🎯 FINAL | Speed: %.0f | State: %d | Crouch: %s"),
                AnimState.Speed,
                (int32)AnimState.MovementState,
                AnimState.bIsCrouching ? TEXT("Yes") : TEXT("No")
            );
            GEngine->AddOnScreenDebugMessage(202, 1.0f, FColor::Green, FinalMsg);
        }
    }
    else
    {
        // Сбрасываем значения если персонажа нет
        AnimState.Speed = 0.0f;
        AnimState.SmoothSpeed = 0.0f;
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = false;
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Unarmed;
        AnimState.MovementState = ES1P_MovementState::Idle;
    }

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
            FString DebugMsg = FString::Printf(TEXT("🔧 DEBUG SPEED: %.0f"), DebugSpeed);
            GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, DebugMsg);
        }
    }

    // Финальная отладка
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
            TEXT("🎮 ANIM: Speed=%.0f | State=%s | Crouch=%s"),
            AnimState.Speed,
            *StateStr,
            AnimState.bIsCrouching ? TEXT("Yes") : TEXT("No")
        );

        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::White, DebugText);
    }
}
void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // Сбрасываем все значения
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Устанавливаем нужное значение в зависимости от оружия
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
    }
}

void UFPSAnimInstance::AnimDebug(int32 Enable)
{
    bDebugMode = (Enable != 0);

    if (GEngine)
    {
        FString Message = bDebugMode ?
            TEXT("✅ ANIM DEBUG: ON (SetTestSpeed <value>)") :
            TEXT("✅ ANIM DEBUG: OFF");

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
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

void UFPSAnimInstance::TestAnimation(FName AnimationName)
{
    ForcedAnimation = AnimationName.ToString();
    bDebugMode = true;

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("🎬 FORCED ANIM: %s"), *ForcedAnimation);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, DebugMsg);
    }
}