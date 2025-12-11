#pragma once

#include "CoreMinimal.h"

// Включаем ТОЛЬКО основные заголовки
#include "Engine/Engine.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"

class FSystem1ParadoxModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};