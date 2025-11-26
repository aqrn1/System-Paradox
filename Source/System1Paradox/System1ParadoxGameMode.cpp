#include "System1ParadoxGameMode.h"
#include "System1ParadoxCharacter.h"
#include "System1ParadoxPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASystem1ParadoxGameMode::ASystem1ParadoxGameMode()
{
    // Настройка класса персонажа по умолчанию
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    else
    {
        // Fallback на C++ класс
        DefaultPawnClass = ASystem1ParadoxCharacter::StaticClass();
    }

    // Настройка PlayerController
    PlayerControllerClass = ASystem1ParadoxPlayerController::StaticClass();
}

void ASystem1ParadoxGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void ASystem1ParadoxGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}