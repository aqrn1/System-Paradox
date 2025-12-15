#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"
#include "System1ParadoxCharacter.h"  // ← ДОБАВЬТЕ ЭТОТ INCLUDE!

#include "FPSAnimInstance.generated.h"

// УДАЛИТЕ: class ASystem1ParadoxCharacter; // Forward declaration
// ДОБАВЬТЕ INCLUDE выше!

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    // ОСНОВНЫЕ ФУНКЦИИ
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override;

    // EXEC КОМАНДЫ
    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    UFUNCTION(Exec, Category = "Animation Debug")
    void TestAnimation(const FString& AnimationName);

    // ПЕРЕМЕННЫЕ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    ASystem1ParadoxCharacter* OwningCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FString ForcedAnimation;

private:
    // ПРИВАТНЫЕ ФУНКЦИИ
    void UpdateWeaponBlendAlphas();
    void UpdateAnimationState(float DeltaSeconds);
    void ApplySmoothing(float DeltaSeconds);
    void ApplyDebugValues();

    float SmoothInterpSpeed;
    bool bForceAnimStateUpdate;
};