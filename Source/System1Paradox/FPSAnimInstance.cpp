#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UFPSAnimInstance::UFPSAnimInstance()
{
    // Инициализация структуры (теперь вызывается конструктор по умолчанию)
    // AnimState уже инициализирована в конструкторе FAnimStateData

    OwningCharacter = nullptr;
    bDebugMode = false;
    DebugSpeed = 0.0f;
    SmoothInterpSpeed = 8.0f;  // Немного уменьшил для более плавных переходов
}

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    if (OwningCharacter && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            TEXT("✅ FPSAnimInstance инициализирован для ") + OwningCharacter->GetName());
    }
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Получаем ссылку на персонажа если еще нет
    if (!OwningCharacter)
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
        if (!OwningCharacter) return;
    }

    // Режим отладки
    if (bDebugMode && DebugSpeed >= 0.0f)
    {
        AnimState.Speed = DebugSpeed;
        AnimState.SmoothSpeed = DebugSpeed;

        // Для отладки устанавливаем стандартные значения
        AnimState.bIsCrouching = false;
        AnimState.bIsSprinting = (DebugSpeed > 400.0f);
        AnimState.bIsInAir = false;
        AnimState.CurrentWeaponType = ES1P_WeaponType::Pistol;
        AnimState.MovementState = (DebugSpeed > 10.0f) ?
            ES1P_MovementState::Walking : ES1P_MovementState::Idle;
    }
    else
    {
        // Нормальный режим
        UpdateAnimationState(DeltaSeconds);
    }

    // Всегда обновляем веса оружия
    UpdateWeaponBlendAlphas();

    // Отладка в реальном времени
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
        }

        FString WeaponStr;
        switch (AnimState.CurrentWeaponType)
        {
        case ES1P_WeaponType::Unarmed: WeaponStr = "Unarmed"; break;
        case ES1P_WeaponType::Pistol: WeaponStr = "Pistol"; break;
        case ES1P_WeaponType::Rifle: WeaponStr = "Rifle"; break;
        case ES1P_WeaponType::Melee: WeaponStr = "Melee"; break;
        }

        FString DebugText = FString::Printf(TEXT("ANIM DEBUG | Speed: %.0f | State: %s | Weapon: %s"),
            AnimState.SmoothSpeed, *StateStr, *WeaponStr);
        GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Yellow, DebugText);
    }
}

void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    if (!OwningCharacter) return;

    // Получаем данные из персонажа
    AnimState.Speed = OwningCharacter->GetCurrentSpeed();
    AnimState.bIsCrouching = OwningCharacter->GetIsCrouching();
    AnimState.bIsSprinting = OwningCharacter->GetIsSprinting();
    AnimState.bIsInAir = OwningCharacter->GetIsInAir();
    AnimState.CurrentWeaponType = OwningCharacter->GetCurrentWeaponType();
    AnimState.MovementState = OwningCharacter->GetMovementState();

    // Применяем сглаживание
    ApplySmoothing(DeltaSeconds);
}

void UFPSAnimInstance::ApplySmoothing(float DeltaSeconds)
{
    // Интерполяция скорости для плавных переходов
    AnimState.SmoothSpeed = FMath::FInterpTo(
        AnimState.SmoothSpeed,
        AnimState.Speed,
        DeltaSeconds,
        SmoothInterpSpeed
    );

    // Ограничиваем скорость для Blend Space
    AnimState.SmoothSpeed = FMath::Clamp(AnimState.SmoothSpeed, 0.0f, 600.0f);
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // Сбрасываем все веса
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Устанавливаем соответствующий вес
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
            TEXT("✅ ANIM DEBUG MODE: ON (используйте SetTestSpeed <значение>)") :
            TEXT("✅ ANIM DEBUG MODE: OFF");
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
    }
}

void UFPSAnimInstance::SetTestSpeed(float NewSpeed)
{
    DebugSpeed = FMath::Clamp(NewSpeed, 0.0f, 1000.0f);
    bDebugMode = true;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("🔧 TEST SPEED SET: %.0f"), DebugSpeed));
    }
}

void UFPSAnimInstance::TestAnimation(FName AnimationName)
{
    ForcedAnimation = AnimationName.ToString();
    bDebugMode = true;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta,
            FString::Printf(TEXT("🔧 FORCED ANIMATION: %s"), *ForcedAnimation));
    }
}