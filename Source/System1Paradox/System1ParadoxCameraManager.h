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

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void UpdateCamera(float DeltaTime) override;

private:
    float DefaultFOV = 90.0f;
};