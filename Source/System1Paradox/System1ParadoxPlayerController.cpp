#include "System1ParadoxPlayerController.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.h"
#include "GameFramework/Pawn.h"

ASystem1ParadoxPlayerController::ASystem1ParadoxPlayerController()
{
    bShowMouseCursor = false;
}

void ASystem1ParadoxPlayerController::BeginPlay()
{
    Super::BeginPlay();

    EnableAnimDebug(true);
}

void ASystem1ParadoxPlayerController::EnableAnimDebug(bool bEnable)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController: Pawn not found"));
        return;
    }

    ASystem1ParadoxCharacter* ControlledCharacter = Cast<ASystem1ParadoxCharacter>(ControlledPawn);
    if (!ControlledCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController: Pawn is not System1ParadoxCharacter"));
        return;
    }

    UFPSAnimInstance* AnimInstance = ControlledCharacter->GetFPSAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController: FPSAnimInstance not found"));
        return;
    }

    AnimInstance->SetDebug(bEnable);
}
