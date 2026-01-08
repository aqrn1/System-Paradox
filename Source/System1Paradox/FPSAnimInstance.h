#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"
#include "FPSAnimInstance.generated.h"

class ASystem1ParadoxCharacter;

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Anim")
    const FS1P_AnimState& GetAnimState() const { return AnimState; }

    // Для отладки анимаций
    UFUNCTION(BlueprintCallable, Category = "Anim")
    void AnimDebug(bool bEnable);

    // Установка тестовой скорости анимации
    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetTestSpeed(float NewSpeed);

private:
    void UpdateFromCharacter();

private:
    UPROPERTY(BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
    FS1P_AnimState AnimState;

    UPROPERTY()
    ASystem1ParadoxCharacter* CachedCharacter;
};
