// System1ParadoxPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "System1ParadoxPlayerController.generated.h"

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASystem1ParadoxPlayerController();

    UFUNCTION(Exec, Category = "Animation Debug")
    void AnimDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Animation Debug")
    void SetTestSpeed(float NewSpeed);
};