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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<class UUserWidget> MainHUDWidgetClass;

protected:
    virtual void BeginPlay() override;

private:
    class UUserWidget* MainHUDWidget;
};