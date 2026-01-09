#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UFPSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // === Debug ===
    void SetDebug(bool bEnable);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    bool bDebugEnabled = false;
};
