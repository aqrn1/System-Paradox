#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"           // Структура FAnimStateData
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    // ========== ОБЯЗАТЕЛЬНЫЕ ПЕРЕОПРЕДЕЛЕНИЯ ==========
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override;

    // ========== EXEC КОМАНДЫ ДЛЯ ОТЛАДКИ ==========
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    UFUNCTION(Exec, Category = "Animation Debug")
    void TestAnimation(const FString& AnimationName);

    // ========== ПУБЛИЧНЫЕ ДАННЫЕ ДЛЯ АНИМАЦИЙ ==========
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    ASystem1ParadoxCharacter* OwningCharacter;

    // ========== НАСТРОЙКИ ОТЛАДКИ ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FString ForcedAnimation;

private:
    // ========== ПРИВАТНЫЕ ФУНКЦИИ ==========
    void UpdateWeaponBlendAlphas();
    void UpdateAnimationState(float DeltaSeconds);
    void ApplySmoothing(float DeltaSeconds);
    void ApplyDebugValues();  // ← ДОБАВЬТЕ ЭТУ СТРОКУ!

    // ========== ПРИВАТНЫЕ ПЕРЕМЕННЫЕ ==========
    float SmoothInterpSpeed;
    bool bForceAnimStateUpdate;
};