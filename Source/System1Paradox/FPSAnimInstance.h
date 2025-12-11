#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override; // ← ДОБАВЬТЕ ЭТУ СТРОКУ!

    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);

    UFUNCTION(Exec, Category = "Animation Debug")
    void TestAnimation(FName AnimationName);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    ASystem1ParadoxCharacter* OwningCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugMode = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FString ForcedAnimation;

private:
    void UpdateAnimationState(float DeltaSeconds);
    void UpdateWeaponBlendAlphas();
    void ApplySmoothing(float DeltaSeconds);

    float SmoothInterpSpeed = 8.0f;
    bool bForceAnimStateUpdate = true;
};