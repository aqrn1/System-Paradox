#include "System1ParadoxGameMode.h"
#include "System1ParadoxCharacter.h"
#include "System1ParadoxPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASystem1ParadoxGameMode::ASystem1ParadoxGameMode()
{
    // Используем наш C++ класс персонажа напрямую
    DefaultPawnClass = ASystem1ParadoxCharacter::StaticClass();

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