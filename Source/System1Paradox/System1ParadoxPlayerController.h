#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "System1ParadoxPlayerController.generated.h"

class UFPSAnimInstance;

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // ✅ ОБЯЗАТЕЛЬНО объявляем конструктор
    ASystem1ParadoxPlayerController();

protected:
    virtual void BeginPlay() override;

public:
    void EnableAnimDebug(bool bEnable);
};
