#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_Types.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

// AAA: Структура для всех данных анимации
USTRUCT(BlueprintType)
struct FAnimStateData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Speed = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsCrouching = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsSprinting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsInAir = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ES1P_WeaponType CurrentWeaponType = ES1P_WeaponType::Unarmed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ES1P_MovementState MovementState = ES1P_MovementState::Idle;

    // AAA: Плавные значения для смешивания
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float SmoothSpeed = 0.0f;

    // AAA: Коэффициенты смешивания для оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float UnarmedAlpha = 1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float PistolAlpha = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float RifleAlpha = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float MeleeAlpha = 0.0f;
};

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // AAA: Функции для консольных команд
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    UFUNCTION(Exec, Category = "Animation Debug")
    void TestAnimation(FName AnimationName);

    // AAA: Основные данные для Blueprint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

    // AAA: Ссылка на персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    ASystem1ParadoxCharacter* OwningCharacter;

    // AAA: Дебаг режим
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = false;

    // AAA: Тестовое значение скорости (для консоли)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSpeed = 0.0f;

    // AAA: Принудительная анимация
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FString ForcedAnimation;

private:
    // AAA: Внутренние функции
    void UpdateAnimationState(float DeltaSeconds);
    void UpdateWeaponBlendAlphas();
    void ApplySmoothing(float DeltaSeconds);

    // AAA: Таймер для плавности
    float SmoothInterpSpeed = 10.0f;
};