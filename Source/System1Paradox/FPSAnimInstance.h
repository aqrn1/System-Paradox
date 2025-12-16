#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"
#include "S1P_Types.h"

// Предварительные объявления
class ASystem1ParadoxCharacter;
class AWeapon;  // ★★★ ДОБАВЬТЕ ЭТУ СТРОКУ ★★★

#include "FPSAnimInstance.generated.h"

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

    // Геттер для Blueprint
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FAnimStateData GetAnimStateData() const { return AnimState; }

    // Основная структура данных
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
    FAnimStateData AnimState;

private:
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