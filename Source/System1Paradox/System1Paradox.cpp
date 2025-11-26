#include "System1Paradox.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    // Код, выполняемый при загрузке модуля
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Started!"));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    // Код, выполняемый при выгрузке модуля
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown!"));
}