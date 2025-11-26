#include "System1ParadoxCameraManager.h"
#include "System1ParadoxCharacter.h"
#include "Engine/Engine.h"

ASystem1ParadoxCameraManager::ASystem1ParadoxCameraManager()
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultFOV = 90.0f;
}

void ASystem1ParadoxCameraManager::BeginPlay()
{
    Super::BeginPlay();
    SetFOV(DefaultFOV);
}

void ASystem1ParadoxCameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASystem1ParadoxCameraManager::UpdateCamera(float DeltaTime)
{
    Super::UpdateCamera(DeltaTime);

    // Применяем настройки FOV для спринта
    if (GetViewTarget())
    {
        ASystem1ParadoxCharacter* PlayerCharacter = Cast<ASystem1ParadoxCharacter>(GetViewTarget());
        if (PlayerCharacter)
        {
            float TargetFOV = PlayerCharacter->GetIsSprinting() ? DefaultFOV + 5.0f : DefaultFOV;
            SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 8.0f));
        }
    }
}