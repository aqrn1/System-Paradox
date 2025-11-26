#include "System1ParadoxCameraManager.h"
#include "System1ParadoxCharacter.h"
#include "Engine/Engine.h"

ASystem1ParadoxCameraManager::ASystem1ParadoxCameraManager()
{
    PrimaryActorTick.bCanEverTick = true;

    // Настройки камеры по умолчанию
    DefaultFOV = FOV;
}

void ASystem1ParadoxCameraManager::BeginPlay()
{
    Super::BeginPlay();
}

void ASystem1ParadoxCameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASystem1ParadoxCameraManager::UpdateCamera(float DeltaTime)
{
    Super::UpdateCamera(DeltaTime);

    // Применяем настройки FOV
    if (GetViewTargetPawn())
    {
        ASystem1ParadoxCharacter* PlayerCharacter = Cast<ASystem1ParadoxCharacter>(GetViewTargetPawn());
        if (PlayerCharacter && PlayerCharacter->GetCameraComponent())
        {
            // Можно добавить логику изменения FOV при беге/прицеливании
            float TargetFOV = PlayerCharacter->GetIsSprinting() ? FOV + 10.0f : FOV;
            SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 8.0f));
        }
    }
}