#include "System1ParadoxCameraManager.h"

ASystem1ParadoxCameraManager::ASystem1ParadoxCameraManager()
{
    // Изначальная настройка камеры, как в CS:GO
    CustomDefaultFOV = 90.0f;
    TargetFOV = CustomDefaultFOV;
    FOVInterpSpeed = 5.0f;
}

void ASystem1ParadoxCameraManager::BeginPlay()
{
    Super::BeginPlay();
    SetCameraFOV(CustomDefaultFOV);
}

void ASystem1ParadoxCameraManager::UpdateCamera(float DeltaTime)
{
    Super::UpdateCamera(DeltaTime);

    // Плавное изменение FOV
    if (FMath::Abs(TargetFOV - CustomDefaultFOV) > KINDA_SMALL_NUMBER)
    {
        CustomDefaultFOV = FMath::FInterpTo(CustomDefaultFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
    }
}

void ASystem1ParadoxCameraManager::SetCameraFOV(float NewFOV)
{
    TargetFOV = NewFOV;
}

void ASystem1ParadoxCameraManager::ResetCameraFOV()
{
    SetCameraFOV(CustomDefaultFOV);
}
