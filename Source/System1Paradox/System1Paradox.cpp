#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "BlueprintManager.h"

// Простая консольная команда для тестирования
static void CreateBlueprintsTest(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🔥 ТЕСТОВАЯ КОМАНДА ВЫЗВАНА!"));

    UBlueprintManager* Manager = NewObject<UBlueprintManager>();
    if (Manager)
    {
        Manager->CreateAndSetupAllBlueprints();
    }
}

static FAutoConsoleCommand CreateBlueprintsTestCmd(
    TEXT("CreateTest"),
    TEXT("Тестовая команда для создания блюпринтов"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CreateBlueprintsTest)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== SYSTEM1PARADOX MODULE STARTED ==="));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown!"));
}