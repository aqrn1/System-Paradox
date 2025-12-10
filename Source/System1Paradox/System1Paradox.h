#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "S1P_Types.h"
#include "S1P_AnimTypes.h" 

class FSystem1ParadoxModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};