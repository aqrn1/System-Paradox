#include "System1ParadoxPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"

ASystem1ParadoxPlayerController::ASystem1ParadoxPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASystem1ParadoxPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input инициализаци€
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // ƒобавление Mapping Context если он настроен
        if (DefaultMappingContext)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ASystem1ParadoxPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void ASystem1ParadoxPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}