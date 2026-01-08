#include "FPSAnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "Weapon.h"  // ★★★ ДОБАВЬТЕ ЭТУ СТРОКУ ★★★
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
        AnimState = FAnimStateData(); // Сброс всех значений
        return;
    }

    // 1. Базовые данные движения
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

    // 2. Состояние движения
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

    // 3. ★★★ ДАННЫЕ ОРУЖИЯ (ПРОВЕРКА НА NULL!) ★★★
    AWeapon* CurrentWeapon = OwningCharacter->GetCurrentWeapon();
    if (CurrentWeapon && IsValid(CurrentWeapon))
    {
        AnimState.bIsFiring = CurrentWeapon->IsFiring();
        AnimState.bIsReloading = CurrentWeapon->IsReloading();
        AnimState.ReloadProgress = CurrentWeapon->GetReloadProgress();

        // Время анимации выстрела
        if (AnimState.bIsFiring)
        {
            AnimState.FireAnimationTime += DeltaSeconds;
            if (AnimState.FireAnimationTime > 0.3f)
                AnimState.FireAnimationTime = 0.0f;
        }
        else
        {
            AnimState.FireAnimationTime = FMath::FInterpTo(AnimState.FireAnimationTime, 0.0f, DeltaSeconds, 10.0f);
        }
    }
    else
    {
        // Оружия нет - сброс значений
        AnimState.bIsFiring = false;
        AnimState.bIsReloading = false;
        AnimState.FireAnimationTime = 0.0f;
        AnimState.ReloadProgress = 0.0f;
    }

    // 4. Blend веса оружия
    UpdateWeaponBlendAlphas();

    // 5. ★★★ ДОПОЛНИТЕЛЬНЫЕ РАСЧЕТЫ ★★★
    if (OwningCharacter.IsValid())
    {
        // Наклон прицеливания
        FRotator AimRotation = OwningCharacter->GetControlRotation();
        FRotator ActorRotation = OwningCharacter->GetActorRotation();
        float DeltaYaw = AimRotation.Yaw - ActorRotation.Yaw;

        // Нормализация угла
        while (DeltaYaw > 180.0f) DeltaYaw -= 360.0f;
        while (DeltaYaw < -180.0f) DeltaYaw += 360.0f;

        AnimState.AimPitch = AimRotation.Pitch;
        AnimState.YawOffset = FMath::FInterpTo(AnimState.YawOffset, DeltaYaw, DeltaSeconds, 8.0f);

        // Направление страфа
        FVector Velocity = OwningCharacter->GetVelocity();
        if (!Velocity.IsNearlyZero())
        {
            FVector Forward = OwningCharacter->GetActorForwardVector();
            FVector Right = OwningCharacter->GetActorRightVector();

            float ForwardSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Forward);
            float RightSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Right);

            AnimState.StrafeDirection = FMath::Atan2(RightSpeed, ForwardSpeed) * (180.0f / PI);
        }
    }

    // 6. DEBUG информация
    if (bDebugMode && GEngine)
    {
        FString DebugMsg = FString::Printf(
            TEXT("AnimState: Wpn=%d | Fire=%s | Reload=%.1f"),
            (int32)AnimState.CurrentWeaponType,
            AnimState.bIsFiring ? TEXT("YES") : TEXT("NO"),
            AnimState.ReloadProgress
        );
        GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Cyan, DebugMsg);
    }
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // Сброс всех значений
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Установка правильного веса в зависимости от оружия
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

void UFPSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Получаем ссылку на владельца персонажа
    OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Проверяем, если персонаж существует
    if (!OwningCharacter.IsValid())
    {
        OwningCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());
    }

    // Обновляем состояние анимации и состояния оружия
    UpdateAnimationState(DeltaSeconds);
    UpdateWeaponBlendAlphas();

    // Применяем отладочные значения, если включена отладка
    if (bDebugMode) ApplyDebugValues();
}


void UFPSAnimInstance::UpdateAnimationState(float DeltaSeconds)
{
    if (!OwningCharacter.IsValid())
    {
        // Если персонаж не существует, сбрасываем все значения анимации
        AnimState = FAnimStateData();
        return;
    }

    // 1. Базовые данные движения
    float Speed = OwningCharacter->GetVelocity().Size2D();
    ES1P_WeaponType WeaponType = OwningCharacter->GetCurrentWeaponType();
    bool bIsCrouching = OwningCharacter->GetIsCrouching();
    bool bIsSprinting = OwningCharacter->GetIsSprinting();
    bool bIsInAir = OwningCharacter->GetIsInAir();

    // Обновляем данные анимации
    AnimState.Speed = Speed;
    AnimState.SmoothSpeed = FMath::FInterpTo(AnimState.SmoothSpeed, Speed, DeltaSeconds, SmoothInterpSpeed);
    AnimState.bIsCrouching = bIsCrouching;
    AnimState.bIsSprinting = bIsSprinting;
    AnimState.bIsInAir = bIsInAir;
    AnimState.CurrentWeaponType = WeaponType;

    // 2. Состояние движения
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

    // 3. Обработка оружия
    AWeapon* CurrentWeapon = OwningCharacter->GetCurrentWeapon();
    if (CurrentWeapon && IsValid(CurrentWeapon))
    {
        AnimState.bIsFiring = CurrentWeapon->IsFiring();
        AnimState.bIsReloading = CurrentWeapon->IsReloading();
        AnimState.ReloadProgress = CurrentWeapon->GetReloadProgress();

        // Время анимации выстрела
        if (AnimState.bIsFiring)
        {
            AnimState.FireAnimationTime += DeltaSeconds;
            if (AnimState.FireAnimationTime > 0.3f)
                AnimState.FireAnimationTime = 0.0f;
        }
        else
        {
            AnimState.FireAnimationTime = FMath::FInterpTo(AnimState.FireAnimationTime, 0.0f, DeltaSeconds, 10.0f);
        }
    }
    else
    {
        // Если оружие отсутствует, сбрасываем значения
        AnimState.bIsFiring = false;
        AnimState.bIsReloading = false;
        AnimState.FireAnimationTime = 0.0f;
        AnimState.ReloadProgress = 0.0f;
    }

    // 4. Обновление весов для анимаций оружия
    UpdateWeaponBlendAlphas();

    // 5. Дополнительные вычисления (например, наклон прицеливания и страйфинг)
    if (OwningCharacter.IsValid())
    {
        // Наклон прицеливания
        FRotator AimRotation = OwningCharacter->GetControlRotation();
        FRotator ActorRotation = OwningCharacter->GetActorRotation();
        float DeltaYaw = AimRotation.Yaw - ActorRotation.Yaw;

        // Нормализуем угол
        while (DeltaYaw > 180.0f) DeltaYaw -= 360.0f;
        while (DeltaYaw < -180.0f) DeltaYaw += 360.0f;

        AnimState.AimPitch = AimRotation.Pitch;
        AnimState.YawOffset = FMath::FInterpTo(AnimState.YawOffset, DeltaYaw, DeltaSeconds, 8.0f);

        // Направление страфа
        FVector Velocity = OwningCharacter->GetVelocity();
        if (!Velocity.IsNearlyZero())
        {
            FVector Forward = OwningCharacter->GetActorForwardVector();
            FVector Right = OwningCharacter->GetActorRightVector();

            float ForwardSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Forward);
            float RightSpeed = FVector::DotProduct(Velocity.GetSafeNormal(), Right);

            AnimState.StrafeDirection = FMath::Atan2(RightSpeed, ForwardSpeed) * (180.0f / PI);
        }
    }

    // 6. Вывод отладочной информации
    if (bDebugMode && GEngine)
    {
        FString DebugMsg = FString::Printf(
            TEXT("AnimState: Wpn=%d | Fire=%s | Reload=%.1f"),
            (int32)AnimState.CurrentWeaponType,
            AnimState.bIsFiring ? TEXT("YES") : TEXT("NO"),
            AnimState.ReloadProgress
        );
        GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Cyan, DebugMsg);
    }
}

void UFPSAnimInstance::UpdateWeaponBlendAlphas()
{
    // Сброс всех значений
    AnimState.UnarmedAlpha = 0.0f;
    AnimState.PistolAlpha = 0.0f;
    AnimState.RifleAlpha = 0.0f;
    AnimState.MeleeAlpha = 0.0f;

    // Установка правильного веса
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

