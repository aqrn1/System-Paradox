#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "S1P_AnimTypes.h"     // структуры состояний
#include "S1P_Types.h"         // enum'ы оружия и движения
#include "CharacterAnimInstance.generated.h"

class ASystem1ParadoxCharacter;

UCLASS()
class SYSTEM1PARADOX_API UCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UCharacterAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    /* ===== ДАННЫЕ ДЛЯ ANIM GRAPH ===== */

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    ES1P_WeaponType WeaponType = ES1P_WeaponType::Unarmed;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsAiming = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    bool bIsReloading = false;

private:
    UPROPERTY(Transient)
    ASystem1ParadoxCharacter* OwningCharacter;

    void UpdateFromCharacter();
};
