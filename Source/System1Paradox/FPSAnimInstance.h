#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UFPSAnimInstance();

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // Переменные для Blueprint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FString MovementState;

private:
    UPROPERTY()
    TWeakObjectPtr<ASystem1ParadoxCharacter> CharacterPtr;

    void UpdateMovementState();
};