#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSystem1ParadoxModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};