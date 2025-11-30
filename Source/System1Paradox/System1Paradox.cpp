#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "BlueprintManager.h"
#include "Engine/World.h"

// КОМАНДЫ ДЛЯ BLUEPRINTS
static void CreateBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🛠️ СОЗДАЕМ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World) return;

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->CreateAllBlueprints();
    }
}

static void AutoBindBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🔗 ПРИВЯЗЫВАЕМ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World) return;

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->AutoBindBlueprints();
    }
}

// ПРОСТЫЕ КОМАНДЫ
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 СИСТЕМА РАБОТАЕТ!"));
}

// РЕГИСТРАЦИЯ КОМАНД
static FAutoConsoleCommand TestCmd(TEXT("sys.Test"), TEXT("Тестовая команда"), FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand));
static FAutoConsoleCommand CreateBPCmd(TEXT("createbp"), TEXT("Создание Blueprints"), FConsoleCommandWithArgsDelegate::CreateStatic(&CreateBlueprintsCommand));
static FAutoConsoleCommand AutoBindCmd(TEXT("autobind"), TEXT("Привязка Blueprints"), FConsoleCommandWithArgsDelegate::CreateStatic(&AutoBindBlueprintsCommand));

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}