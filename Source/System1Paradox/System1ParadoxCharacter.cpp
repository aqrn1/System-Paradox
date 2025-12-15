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

void ASystem1ParadoxCharacter::SpawnDefaultWeapon()
{
    if (!DefaultWeaponClass)
    {
        UE_LOG(LogTemp, Error, TEXT("System1ParadoxCharacter: DefaultWeaponClass is not set!"));
        return;
    }

    // Используем отложенное создание через таймер
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            // Проверяем компоненты
            if (!IsValid(this) || !GetWorld() || !CameraComponent)
            {
                UE_LOG(LogTemp, Error, TEXT("System1ParadoxCharacter: Cannot spawn weapon - invalid state"));
                return;
            }

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // Спавним оружие
            FVector SpawnLocation = GetActorLocation();
            FRotator SpawnRotation = GetActorRotation();

            CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass, SpawnLocation, SpawnRotation, SpawnParams);

            if (CurrentWeapon)
            {
                // Прикрепляем оружие к камере
                FAttachmentTransformRules AttachmentRules(
                    EAttachmentRule::SnapToTarget,
                    EAttachmentRule::SnapToTarget,
                    EAttachmentRule::KeepWorld,
                    true
                );

                CurrentWeapon->AttachToComponent(CameraComponent, AttachmentRules);

                // Настройка позиции оружия от первого лица
                CurrentWeapon->SetActorRelativeLocation(FVector(35.0f, 8.0f, -30.0f));
                CurrentWeapon->SetActorRelativeRotation(FRotator(2.0f, -95.0f, -5.0f));
                CurrentWeapon->SetActorScale3D(FVector(0.8f));

                CurrentWeaponType = ES1P_WeaponType::Pistol;

                UE_LOG(LogTemp, Log, TEXT("✅ System1ParadoxCharacter: Weapon spawned and attached successfully"));

                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                        TEXT("✅ WEAPON EQUIPPED AND ATTACHED TO CAMERA"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("System1ParadoxCharacter: Failed to spawn weapon!"));
            }

        }, 0.2f, false); // Задержка 200ms для гарантированной инициализации
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