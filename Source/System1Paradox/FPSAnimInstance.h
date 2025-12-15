// FPSAnimInstance.h - ПОЛНОСТЬЮ ИСПРАВЛЕННЫЙ ФАЙЛ
#pragma once

// 1. Сначала стандартные includes Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

// 2. Затем ВСЕ наши пользовательские includes
#include "S1P_AnimTypes.h"  // Структура данных для анимаций
#include "S1P_Types.h"      // Перечисления WeaponType и MovementState

// 3. В САМОМ КОНЦЕ - generated.h (ЭТО ОБЯЗАТЕЛЬНО!)
#include "FPSAnimInstance.generated.h"

// 4. Предварительное объявление (после всех includes)
class ASystem1ParadoxCharacter;

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override;

    // Консольные команды
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    // Геттер для получения данных от персонажа
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FAnimStateData GetAnimStateData() const { return AnimState; }

    // Основная структура данных для Blueprint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

private:
    // Указатель на персонажа - только предварительное объявление
    TWeakObjectPtr<ASystem1ParadoxCharacter> OwningCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bDebugMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    float DebugSpeed;

    float SmoothInterpSpeed;

    void UpdateWeaponBlendAlphas();
    void UpdateAnimationState(float DeltaSeconds);
    void ApplyDebugValues();
};