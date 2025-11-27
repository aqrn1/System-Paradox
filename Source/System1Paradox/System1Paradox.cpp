#include "System1Paradox.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== SYSTEM1PARADOX MODULE STARTED ==="));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown!"));
}