#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"  // ИЗМЕНЕНИЕ: Включаем новый файл
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

// ВАЖНО: УДАЛИТЬ структуру FAnimStateData отсюда полностью!

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    /** Консольная команда для включения/выключения режима отладки анимаций */
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    /** Консольная команда для установки тестовой скорости */
    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    /** Консольная команда для тестирования конкретной анимации */
    UFUNCTION(Exec, Category = "Animation Debug")
    void TestAnimation(FName AnimationName);

    /** Основная структура с данными состояния анимации для Blueprint */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

    /** Ссылка на владельца (персонажа) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    ASystem1ParadoxCharacter* OwningCharacter;

    /** Режим отладки (включается через консоль) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = false;

    /** Скорость для режима отладки */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSpeed = 0.0f;

    /** Имя анимации для принудительного воспроизведения */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FString ForcedAnimation;

private:
    /** Обновление состояния анимации на основе данных персонажа */
    void UpdateAnimationState(float DeltaSeconds);

    /** Обновление весов оружия для смешивания анимаций */
    void UpdateWeaponBlendAlphas();

    /** Применение сглаживания к скорости */
    void ApplySmoothing(float DeltaSeconds);

    /** Скорость интерполяции для плавности */
    float SmoothInterpSpeed = 10.0f;
};