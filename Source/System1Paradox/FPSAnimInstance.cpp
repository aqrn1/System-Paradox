#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    OwningCharacter = nullptr;
    bDebugMode = false;
    DebugSpeed = 0.0f;
    SmoothInterpSpeed = 10.0f;
}

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Получаем ссылку на персонажа при инициализации
    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (OwningCharacter && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            TEXT("🎬 AAA AnimInstance Initialized"));
    }
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Обновляем ссылку на персонажа каждый кадр
    if (!OwningCharacter)
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
    }

    // Если нет персонажа - сброс
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

    // Режим дебага: используем тестовые значения из консоли
    if (bDebugMode && DebugSpeed >= 0.0f)
    {
        AnimState.Speed = DebugSpeed;
        AnimState.SmoothSpeed = DebugSpeed;
    }
    else
    {
        // Нормальный режим: получаем данные от персонажа
        UpdateAnimationState(DeltaSeconds);
    }

    // Обновляем коэффициенты смешивания для оружия
    UpdateWeaponBlendAlphas();

    // Дебаг информация
    if (bDebugMode && GEngine)
    {
        FString DebugText = FString::Printf(
            TEXT("ANIM DEBUG | Speed: %.0f | State: %d | Weapon: %d"),
            AnimState.Speed,
            (int32)AnimState.MovementState,
            (int32)AnimState.CurrentWeaponType
        );

        GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Yellow, DebugText);
    }
}

void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    // Получаем ВСЕ данные от персонажа
    AnimState.Speed = OwningCharacter->GetCurrentSpeed();
    AnimState.bIsCrouching = OwningCharacter->GetIsCrouching();
    AnimState.bIsSprinting = OwningCharacter->GetIsSprinting();
    AnimState.bIsInAir = OwningCharacter->GetIsInAir();
    AnimState.CurrentWeaponType = OwningCharacter->GetCurrentWeaponType();
    AnimState.MovementState = OwningCharacter->GetMovementState();

    // AAA: Плавное изменение скорости для красивых переходов
    ApplySmoothing(DeltaSeconds);
}

void UFPSAnimInstance::ApplySmoothing(float DeltaSeconds)
{
    // Плавная интерполяция скорости
    AnimState.SmoothSpeed = FMath::FInterpTo(
        AnimState.SmoothSpeed,
        AnimState.Speed,
        DeltaSeconds,
        SmoothInterpSpeed
    );
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // AAA: Сбрасываем все альфы
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Устанавливаем альфу для текущего оружия
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

// ==================== КОНСОЛЬНЫЕ КОМАНДЫ ====================

void UFPSAnimInstance::AnimDebug(int32 Enable)
{
    bDebugMode = (Enable != 0);

    if (GEngine)
    {
        FString Message = bDebugMode ?
            TEXT("🔧 ANIM DEBUG MODE: ON") :
            TEXT("🔧 ANIM DEBUG MODE: OFF");

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
    }
}

void UFPSAnimInstance::SetTestSpeed(float NewSpeed)
{
    DebugSpeed = FMath::Clamp(NewSpeed, 0.0f, 1000.0f);
    bDebugMode = true;

    if (GEngine)
    {
        FString Message = FString::Printf(
            TEXT("🔧 TEST SPEED SET: %.0f"),
            DebugSpeed
        );
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
    }
}

void UFPSAnimInstance::TestAnimation(FName AnimationName)
{
    ForcedAnimation = AnimationName.ToString();
    bDebugMode = true;

    if (GEngine)
    {
        FString Message = FString::Printf(
            TEXT("🔧 FORCED ANIMATION: %s"),
            *ForcedAnimation
        );
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Message);
    }
}