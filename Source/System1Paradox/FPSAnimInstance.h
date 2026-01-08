#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"
#include "S1P_Types.h"

// Предварительные объявления
class ASystem1ParadoxCharacter;
class AWeapon;  // Для работы с оружием

#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    // Инициализация анимации
    virtual void NativeInitializeAnimation() override;

    // Обновление анимации
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // Очищаем данные
    virtual void NativeUninitializeAnimation() override;

    // Консольные команды для отладки анимаций
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    // Геттер для состояния анимации
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FAnimStateData GetAnimStateData() const { return AnimState; }

    // Структура для хранения данных анимации
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

private:
    // Ссылка на персонажа
    TWeakObjectPtr<ASystem1ParadoxCharacter> OwningCharacter;

    // Флаг для отладки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bDebugMode;

    // Тестовая скорость для отладки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    float DebugSpeed;

    // Коэффициент интерполяции для сглаживания
    float SmoothInterpSpeed;

    // Функции для обновления данных анимации
    void UpdateWeaponBlendAlphas();
    void UpdateAnimationState(float DeltaSeconds);
    void ApplyDebugValues();
};
