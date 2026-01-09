#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "System1ParadoxPlayerController.generated.h"

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // ¬ключить / выключить дебаг анимаций
    UFUNCTION(BlueprintCallable)
    void ToggleAnimDebug(int32 Enable);

    UFUNCTION(BlueprintCallable)
    void ToggleAnimDebugOff();
};
