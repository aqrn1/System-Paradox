// System1ParadoxPlayerController.cpp
#include "System1ParadoxCharacter.h"
#include "System1ParadoxPlayerController.h"
#include "System1ParadoxHUD.h"
#include "FPSAnimInstance.h"
#include "Weapon.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ASystem1ParadoxPlayerController::ASystem1ParadoxPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = false;
    UE_LOG(LogTemp, Warning, TEXT("🎮 PlayerController Created"));
}

void ASystem1ParadoxPlayerController::AnimDebug(int32 Enable)
{
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

// ПРОСТАЯ ВЕРСИЯ DebugWeaponPos (без доступа к protected)
void ASystem1ParadoxPlayerController::DebugWeaponPos()
{
    APlayerController::DebugWeaponPos(); // Вызов родительской если есть

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
            TEXT("🎯 Debug Weapon Positions - Check character position"));
    }
}

// ПРОСТАЯ ВЕРСИЯ DebugReload
void ASystem1ParadoxPlayerController::DebugReload()
{
    ASystem1ParadoxCharacter* MyCharacter = Cast<ASystem1ParadoxCharacter>(GetPawn());
    if (MyCharacter)
    {
        // Вызываем публичный метод персонажа
        MyCharacter->ReloadWeapon();

        UE_LOG(LogTemp, Warning, TEXT("🛠️ DEBUG: Reload command sent"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta,
                TEXT("🛠️ DEBUG: Reload command sent to character"));
        }
    }
}

// ПРОСТАЯ ВЕРСИЯ DebugAmmo
void ASystem1ParadoxPlayerController::DebugAmmo(int32 NewAmmo)
{
    UE_LOG(LogTemp, Warning, TEXT("🛠️ DEBUG: Ammo command received: %d"), NewAmmo);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
            FString::Printf(TEXT("🛠️ DEBUG: Ammo set to %d (command received)"), NewAmmo));
    }
}

// ПРОСТАЯ ВЕРСИЯ ShowMessage
void ASystem1ParadoxPlayerController::ShowMessage(const FString& Message)
{
    // Просто показываем сообщение на экране
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
            FString::Printf(TEXT("📢 Message: %s"), *Message));
    }

    UE_LOG(LogTemp, Warning, TEXT("📢 Console Message: %s"), *Message);
}