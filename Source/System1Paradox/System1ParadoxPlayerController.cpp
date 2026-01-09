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

void ASystem1ParadoxPlayerController::DebugPlayer()
{
    ASystem1ParadoxCharacter* Character =
        Cast<ASystem1ParadoxCharacter>(GetPawn());

    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("DebugPlayer: Character = NULL"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== DEBUG PLAYER ==="));
    UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Character->GetCurrentSpeed());
    UE_LOG(LogTemp, Warning, TEXT("Sprinting: %s"), Character->GetIsSprinting() ? TEXT("YES") : TEXT("NO"));
    UE_LOG(LogTemp, Warning, TEXT("Crouching: %s"), Character->GetIsCrouching() ? TEXT("YES") : TEXT("NO"));
    UE_LOG(LogTemp, Warning, TEXT("InAir: %s"), Character->GetIsInAir() ? TEXT("YES") : TEXT("NO"));
}