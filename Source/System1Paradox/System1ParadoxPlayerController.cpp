#include "System1ParadoxPlayerController.h"

// UE
#include "GameFramework/Pawn.h"

// Game
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.h"

void ASystem1ParadoxPlayerController::ToggleAnimDebug(int32 Enable)
{
    // ВСЕГДА получаем персонажа через GetPawn()
    ASystem1ParadoxCharacter* S1PCharacter =
        Cast<ASystem1ParadoxCharacter>(GetPawn());

    if (!S1PCharacter)
    {
        return;
    }

    UFPSAnimInstance* AnimInstance = S1PCharacter->GetFPSAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    AnimInstance->AnimDebug(Enable != 0);
}

void ASystem1ParadoxPlayerController::ToggleAnimDebugOff()
{
    ASystem1ParadoxCharacter* S1PCharacter =
        Cast<ASystem1ParadoxCharacter>(GetPawn());

    if (!S1PCharacter)
    {
        return;
    }

    UFPSAnimInstance* AnimInstance = S1PCharacter->GetFPSAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    AnimInstance->AnimDebug(false);
}
