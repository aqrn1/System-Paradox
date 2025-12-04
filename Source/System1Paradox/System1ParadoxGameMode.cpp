#include "System1ParadoxGameMode.h"
#include "System1ParadoxCharacter.h"
#include "System1ParadoxPlayerController.h"
#include "System1ParadoxHUD.h"
#include "Blueprint/UserWidget.h"

ASystem1ParadoxGameMode::ASystem1ParadoxGameMode()
{
    DefaultPawnClass = ASystem1ParadoxCharacter::StaticClass();
    PlayerControllerClass = ASystem1ParadoxPlayerController::StaticClass();
    HUDClass = ASystem1ParadoxHUD::StaticClass(); // ДОБАВЬТЕ ЭТУ СТРОКУ
}

void ASystem1ParadoxGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Создаем HUD виджет если он задан
    if (MainHUDWidgetClass)
    {
        MainHUDWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);
        if (MainHUDWidget)
        {
            MainHUDWidget->AddToViewport();
        }
    }
}

