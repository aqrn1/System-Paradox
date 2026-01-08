#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "System1ParadoxCameraManager.generated.h"

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCameraManager();

    // Настройки камеры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float CustomDefaultFOV = 90.0f;  // Базовое поле зрения камеры (с измененным именем)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float TargetFOV = 90.0f;   // Целевое поле зрения (для анимаций изменения FOV)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float FOVInterpSpeed = 5.0f; // Скорость изменения FOV

    // Обновление состояния камеры каждый кадр
    virtual void UpdateCamera(float DeltaTime) override;

    // Функции для изменения FOV
    void SetCameraFOV(float NewFOV);
    void ResetCameraFOV();

protected:
    virtual void BeginPlay() override;
};
