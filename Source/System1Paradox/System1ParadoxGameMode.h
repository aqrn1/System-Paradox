#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "System1ParadoxGameMode.generated.h"

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASystem1ParadoxGameMode();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};