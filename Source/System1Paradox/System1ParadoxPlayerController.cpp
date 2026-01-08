#include "System1ParadoxPlayerController.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.h"

ASystem1ParadoxPlayerController::ASystem1ParadoxPlayerController()
{
    bShowMouseCursor = false;
}

void ASystem1ParadoxPlayerController::AnimDebug(int32 Enable)
{
    ASystem1ParadoxCharacter* Character = Cast<ASystem1ParadoxCharacter>(GetPawn());
    if (!Character)
    {
        return;
    }

    UFPSAnimInstance* AnimInstance = Character->GetFPSAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    AnimInstance->AnimDebug(Enable);
}

void ASystem1ParadoxPlayerController::SetTestSpeed(float NewSpeed)
{
    ASystem1ParadoxCharacter* Character = Cast<ASystem1ParadoxCharacter>(GetPawn());
    if (!Character)
    {
        return;
    }

    UFPSAnimInstance* AnimInstance = Character->GetFPSAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    AnimInstance->SetTestSpeed(NewSpeed);
}
