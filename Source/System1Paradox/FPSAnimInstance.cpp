#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

// ==================== КОНСТРУКТОР ====================
UFPSAnimInstance::UFPSAnimInstance()
{
    OwningCharacter = nullptr;
    bDebugMode = false;
    DebugSpeed = 0.0f;
    ForcedAnimation = TEXT("");
    SmoothInterpSpeed = 8.0f;
    bForceAnimStateUpdate = true;
}

// ==================== ИНИЦИАЛИЗАЦИЯ ====================
void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Получаем ссылку на персонажа при инициализации
    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (GEngine && OwningCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("✅ ANIM INIT: %s"), *OwningCharacter->GetName()));
    }
}

// ==================== ОБНОВЛЕНИЕ АНИМАЦИИ (ГЛАВНАЯ ФУНКЦИЯ) ====================
void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // 1. ПОЛУЧАЕМ ССЫЛКУ НА ПЕРСОНАЖА (если еще нет)
    if (!OwningCharacter)
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

        if (OwningCharacter && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("✅ CHARACTER FOUND: %s"), *OwningCharacter->GetName()));
        }
    }

    // 2. ОБНОВЛЯЕМ СОСТОЯНИЕ АНИМАЦИИ
    UpdateAnimationState(DeltaSeconds);

    // 3. ОБНОВЛЯЕМ СМЕШИВАНИЕ ОРУЖИЯ
    UpdateWeaponBlendAlphas();

    // 4. ОТЛАДКА (если включена)
    if (bDebugMode)
    {
        ApplyDebugValues();  // ← ТЕПЕРЬ ЭТА ФУНКЦИЯ ОБЪЯВЛЕНА В .h
    }
}

// ==================== ДЕИНИЦИАЛИЗАЦИЯ ====================
void UFPSAnimInstance::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();

    // Сбрасываем ссылку при деинициализации
    OwningCharacter = nullptr;
}

// ==================== ОБНОВЛЕНИЕ СОСТОЯНИЯ АНИМАЦИИ ====================
void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    // Если нет персонажа - сбрасываем значения
    if (!OwningCharacter)
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

    // Получаем данные от персонажа
    float Speed = OwningCharacter->GetVelocity().Size2D();
    ES1P_WeaponType WeaponType = OwningCharacter->GetCurrentWeaponType();
    bool bIsCrouching = OwningCharacter->GetIsCrouching();
    bool bIsSprinting = OwningCharacter->GetIsSprinting();
    bool bIsInAir = OwningCharacter->GetIsInAir();

    // Применяем сглаживание скорости
    AnimState.Speed = Speed;
    AnimState.SmoothSpeed = FMath::FInterpTo(AnimState.SmoothSpeed, Speed, DeltaSeconds, SmoothInterpSpeed);

    // Устанавливаем состояния
    AnimState.bIsCrouching = bIsCrouching;
    AnimState.bIsSprinting = bIsSprinting;
    AnimState.bIsInAir = bIsInAir;
    AnimState.CurrentWeaponType = WeaponType;

    // Определяем состояние движения
    if (AnimState.bIsInAir)
    {
        AnimState.MovementState = ES1P_MovementState::Jumping;
    }
    else if (AnimState.bIsCrouching)
    {
        AnimState.MovementState = ES1P_MovementState::Crouching;
    }
    else if (AnimState.bIsSprinting && Speed > 100.0f)
    {
        AnimState.MovementState = ES1P_MovementState::Sprinting;
    }
    else if (Speed > 10.0f)
    {
        AnimState.MovementState = ES1P_MovementState::Walking;
    }
    else
    {
        AnimState.MovementState = ES1P_MovementState::Idle;
    }

    // Отладка в консоль (раз в 2 секунды)
    static float DebugTimer = 0.0f;
    DebugTimer += DeltaSeconds;
    if (DebugTimer >= 2.0f)
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

        if (GEngine)
        {
            FString DebugMsg = FString::Printf(
                TEXT("🎮 ANIM: Speed=%.0f | State=%s | Weapon=%d"),
                AnimState.Speed,
                *StateStr,
                (int32)AnimState.CurrentWeaponType
            );
            GEngine->AddOnScreenDebugMessage(1, 2.1f, FColor::White, DebugMsg);
        }

        DebugTimer = 0.0f;
    }
}

// ==================== ОБНОВЛЕНИЕ СМЕШИВАНИЯ ОРУЖИЯ ====================
void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // Сбрасываем все альфа-значения
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Устанавливаем альфа для текущего оружия
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

// ==================== ПРИМЕНЕНИЕ СГЛАЖИВАНИЯ ====================
void UFPSAnimInstance::ApplySmoothing(float DeltaSeconds)
{
    // Базовая реализация сглаживания
    if (OwningCharacter)
    {
        float TargetSpeed = OwningCharacter->GetVelocity().Size2D();
        AnimState.SmoothSpeed = FMath::FInterpTo(AnimState.SmoothSpeed, TargetSpeed, DeltaSeconds, SmoothInterpSpeed);
    }
}

// ==================== ПРИМЕНЕНИЕ ОТЛАДОЧНЫХ ЗНАЧЕНИЙ ====================
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

// ==================== EXEC КОМАНДЫ ====================

// AnimDebug <0/1> - включение/выключение отладки
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

// SetTestSpeed <скорость> - установка тестовой скорости
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

// TestAnimation <имя> - принудительная анимация
void UFPSAnimInstance::TestAnimation(const FString& AnimationName)
{
    ForcedAnimation = AnimationName;
    bDebugMode = true;

    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("🎬 FORCED ANIM: %s"), *ForcedAnimation);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, DebugMsg);
    }
}