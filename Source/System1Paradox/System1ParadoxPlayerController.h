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

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

public:
    virtual void Tick(float DeltaTime) override;
};