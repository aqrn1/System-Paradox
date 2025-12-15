// System1ParadoxPlayerController.cpp
#include "System1ParadoxPlayerController.h"
#include "System1ParadoxCharacter.h"
#include "FPSAnimInstance.h"
#include "Engine/Engine.h"

void ASystem1ParadoxPlayerController::AnimDebug(int32 Enable)
{
    // Измените имя переменной (не Character!)
    ASystem1ParadoxCharacter* MyCharacter = Cast<ASystem1ParadoxCharacter>(GetPawn());
    if (!MyCharacter)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("❌ ANIM DEBUG: Нет персонажа"));
        return;
    }

    UFPSAnimInstance* AnimInst = MyCharacter->GetFPSAnimInstance();
    if (AnimInst)
    {
        AnimInst->AnimDebug(Enable);
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("✅ PC: AnimDebug %d"), Enable));
    }
}

void ASystem1ParadoxPlayerController::SetTestSpeed(float NewSpeed)
{
    ASystem1ParadoxCharacter* MyCharacter = Cast<ASystem1ParadoxCharacter>(GetPawn());
    if (!MyCharacter) return;

    UFPSAnimInstance* AnimInst = MyCharacter->GetFPSAnimInstance();
    if (AnimInst)
    {
        AnimInst->SetTestSpeed(NewSpeed);
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("✅ PC: SetTestSpeed %.0f"), NewSpeed));
    }
}